package com.igorion.bme;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import com.fasterxml.jackson.core.util.DefaultIndenter;
import com.fasterxml.jackson.core.util.DefaultPrettyPrinter;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.igorion.mqtt.types.bmerawdata.JsonTypeImplBmeDataColumn;
import com.igorion.mqtt.types.bmerawdata.JsonTypeImplBmeDutyCycleProfile;
import com.igorion.mqtt.types.bmerawdata.JsonTypeImplBmeHeaterProfile;
import com.igorion.mqtt.types.bmerawdata.JsonTypeImplBmeSensorConfiguration;
import com.igorion.mqtt.types.bmerawdata.JsonTypeImplBmerawdata;
import com.igorion.mqtt.types.message.JsonTypeImplBmeHeater;
import com.igorion.mqtt.types.message.JsonTypeImplBmeValues;

/**
 * simple wrapper around a {@link JsonTypeImplBmerawdata} instance
 *
 * @author h.fleischer
 * @since 18.08.2023
 *
 */
public class BmeRawData {

    protected static final SimpleDateFormat BMERAWDATA_FILE_NAME_FORMAT = new SimpleDateFormat("yyyy_MM_dd_HH_mm");
    protected static final SimpleDateFormat BMERAWDATA_DATA_HEADER_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssZ"); // 2023-08-10T05:24:49+02:00
    protected static final SimpleDateFormat BMERAWDATA_CONFIG_HEADER_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'.000Z'");
    static {
        BMERAWDATA_CONFIG_HEADER_FORMAT.setTimeZone(TimeZone.getTimeZone("UTC"));
    }

    private final JsonTypeImplBmerawdata bmerawdataJo;
    private final Date fileDate;
    private final String fileName;

    public final int refSensorIndex;
    public final int refSensorId;
    public final long refTimeSincePowerOn;

    public BmeRawData(byte[] payload) {

        try {

            JsonTypeImplBmeHeater bmeHeaterMessage = new ObjectMapper().readValue(payload, JsonTypeImplBmeHeater.class);
            this.fileDate = new Date(); // let the first profile be the reference date for the file

            this.bmerawdataJo = new JsonTypeImplBmerawdata();
            this.fileName = String.format("%s_Board_%s_PowerOnOff_1_%s_File_0.bmerawdata",
                        BMERAWDATA_FILE_NAME_FORMAT.format(this.fileDate),
                        this.bmerawdataJo.rawDataHeader.boardId,
                        this.bmerawdataJo.rawDataHeader.seedPowerOnOff);

            this.bmerawdataJo.configHeader.dateCreated = BMERAWDATA_CONFIG_HEADER_FORMAT.format(this.fileDate);
            this.bmerawdataJo.rawDataHeader.dateCreated = String.valueOf(this.fileDate.getTime() / 1000);
            this.bmerawdataJo.rawDataHeader.dateCreated_ISO = BMERAWDATA_DATA_HEADER_FORMAT.format(this.fileDate);

            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Sensor Index", "", "integer", "sensor_index"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Sensor ID", "", "integer", "sensor_id"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Time Since PowerOn", "Milliseconds", "integer", "timestamp_since_poweron"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Real time clock", "Unix Timestamp: seconds since Jan 01 1970. (UTC); 0 = missing", "integer", "real_time_clock"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Temperature", "DegreesCelcius", "float", "temperature"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Pressure", "Hectopascals", "float", "pressure"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Relative Humidity", "Percent", "float", "relative_humidity"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Resistance Gassensor", "Ohms", "float", "resistance_gassensor"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Heater Profile Step Index", "", "integer", "heater_profile_step_index"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Scanning Mode Enabled", "", "integer", "scanning_mode_enabled"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Label Tag", "", "integer", "label_tag"));
            this.bmerawdataJo.rawDataBody.dataColumns.add(createDataColumn("Error Code", "", "integer", "error_code"));

            this.refTimeSincePowerOn = bmeHeaterMessage.timeSincePowerOn;
            this.refSensorId = bmeHeaterMessage.sensorId;
            this.refSensorIndex = 0; // bmeHeaterMessage.sensorIndex;

            // config body - heater profile
            JsonTypeImplBmeHeaterProfile heaterProfileJo = new JsonTypeImplBmeHeaterProfile();
            heaterProfileJo.id = bmeHeaterMessage.profileId;
            for (int i = 0; i < 10; i++) {
                heaterProfileJo.temperatureTimeVectors.add(new int[] { bmeHeaterMessage.steps.get(i * 2), bmeHeaterMessage.steps.get(i * 2 + 1) });
            }
            this.bmerawdataJo.configBody.heaterProfiles.add(heaterProfileJo);

            // config body - duty cycle profile
            JsonTypeImplBmeDutyCycleProfile dutyCycleProfileJo = new JsonTypeImplBmeDutyCycleProfile();
            this.bmerawdataJo.configBody.dutyCycleProfiles.add(dutyCycleProfileJo);

            // config body - sensor configuration
            JsonTypeImplBmeSensorConfiguration sensorConfigurationJo = new JsonTypeImplBmeSensorConfiguration();
            sensorConfigurationJo.sensorIndex = this.refSensorIndex;
            sensorConfigurationJo.heaterProfile = heaterProfileJo.id;
            sensorConfigurationJo.dutyCycleProfile = dutyCycleProfileJo.id;
            this.bmerawdataJo.configBody.sensorConfigurations.add(sensorConfigurationJo);

            persist();

        } catch (Exception ex) {
            throw new RuntimeException("failed to create bmerawdata", ex);
        }

    }

    public void acceptData(byte[] payload) {

        try {

            JsonTypeImplBmeValues bmeValuesMessage = new ObjectMapper().readValue(payload, JsonTypeImplBmeValues.class);

            long valTimeSincePowerOn = bmeValuesMessage.timeSincePowerOn - this.refTimeSincePowerOn;
            long unixTimestamp = (this.fileDate.getTime() + valTimeSincePowerOn) / 1000;

            this.bmerawdataJo.rawDataBody.dataBlock.add(new Number[] {
                        this.refSensorIndex,
                        this.refSensorId,
                        valTimeSincePowerOn,
                        unixTimestamp, // TODO :: calculate from reference and offset
                        bmeValuesMessage.temperature,
                        bmeValuesMessage.pressure,
                        bmeValuesMessage.humidity,
                        bmeValuesMessage.gasResistance,
                        bmeValuesMessage.heaterStepIndex,
                        1,
                        bmeValuesMessage.specimenIndex,
                        0
            });
            if (this.bmerawdataJo.rawDataBody.dataBlock.size() % 10 == 0) {
                persist();
            }

        } catch (Exception ex) {
            throw new RuntimeException("failed to accept data", ex);
        }

    }

    protected static JsonTypeImplBmeDataColumn createDataColumn(String name, String unit, String format, String key) {

        JsonTypeImplBmeDataColumn dataColumnJo = new JsonTypeImplBmeDataColumn();
        dataColumnJo.name = name;
        dataColumnJo.unit = unit;
        dataColumnJo.format = format;
        dataColumnJo.key = key;
        return dataColumnJo;

    }

    public void persist() {

        try {

            // sort sensor configs by sensor id
            this.bmerawdataJo.configBody.sensorConfigurations.sort((a, b) -> {
                return a.sensorIndex - b.sensorIndex;
            });

            // sort all values, regardless of sensor id by timeSincePowerOn
            this.bmerawdataJo.rawDataBody.dataBlock.sort((a, b) -> {
                return a[2].intValue() - b[2].intValue();
            });

            ObjectMapper mapper = new ObjectMapper();
            DefaultPrettyPrinter prettyPrinter = new DefaultPrettyPrinter();
            prettyPrinter.indentArraysWith(DefaultIndenter.SYSTEM_LINEFEED_INSTANCE);
            mapper.setDefaultPrettyPrinter(prettyPrinter);
            mapper.writerWithDefaultPrettyPrinter().writeValue(new File("C:\\BME-Ai-Studio\\data", this.fileName), this.bmerawdataJo);

        } catch (Exception ex) {
            throw new RuntimeException("failed to persist bmerawdata", ex);
        }

    }

}
