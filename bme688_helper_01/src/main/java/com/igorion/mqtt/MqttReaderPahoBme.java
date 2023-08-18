package com.igorion.mqtt;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttSecurityException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import com.fasterxml.jackson.core.JsonGenerationException;
import com.fasterxml.jackson.core.util.DefaultIndenter;
import com.fasterxml.jackson.core.util.DefaultPrettyPrinter;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.igorion.mqtt.type.JsonTypeImplBmeDataColumn;
import com.igorion.mqtt.type.JsonTypeImplBmeDutyCycleProfile;
import com.igorion.mqtt.type.JsonTypeImplBmeHeater;
import com.igorion.mqtt.type.JsonTypeImplBmeHeaterProfile;
import com.igorion.mqtt.type.JsonTypeImplBmeSensorConfiguration;
import com.igorion.mqtt.type.JsonTypeImplBmeValues;
import com.igorion.mqtt.type.JsonTypeImplBmerawdata;

public class MqttReaderPahoBme {

    public static final String MQTT_HOST = "tcp://192.168.0.115:1883";
    public static final String MQTT_CLID = "bme_helper";

    /**
     * vars that get set with the first profile coming in
     */
    public static Date bmerawdataFileDate;
    public static JsonTypeImplBmerawdata bmerawdata;
    public static String bmerawdataName;

    public static int refSensorIndex;
    public static int refSensorId;
    public static long refTimeSincePowerOn;
    public static long timeSincePowerOnMax = Long.MAX_VALUE;
    public static boolean excludeForRepeatedHeaterProfileViolation = false;
    public static boolean excludeForTimeSincePowerOnViolation = false;

    public static final SimpleDateFormat BMERAWDATA_FILE_NAME_FORMAT = new SimpleDateFormat("yyyy_MM_dd_HH_mm");
    public static final SimpleDateFormat BMERAWDATA_CONFIG_HEADER_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'.000Z'");
    public static final SimpleDateFormat BMERAWDATA_DATA_HEADER_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssZ"); // 2023-08-10T05:24:49+02:00

    static {
        BMERAWDATA_CONFIG_HEADER_FORMAT.setTimeZone(TimeZone.getTimeZone("UTC"));
    }

    @SuppressWarnings("resource")
    public static void main(String[] args) throws Exception {

        MqttClient client = new MqttClient(MQTT_HOST, MQTT_CLID, new MemoryPersistence());
        MqttConnectOptions options = new MqttConnectOptions();
        options.setHttpsHostnameVerificationEnabled(false);

        client.setCallback(new MqttCallback() {

            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("connectionLost: " + cause.getMessage() + ", trying to reconnect");
                try {
                    connect(client, options);
                } catch (Exception ex) {
                    throw new RuntimeException("failed to reonnect", ex);
                }
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {

                try {

                    if (topic.equals("bmeheater")) {

                        System.out.println("received heater profile " + (bmerawdata != null ? "(additional)" : "(first)"));

                        java.awt.Toolkit.getDefaultToolkit().beep();

                        // read the heater profile, then create a new bmerawdata object
                        JsonTypeImplBmeHeater bmeHeaterMessage = new ObjectMapper().readValue(message.getPayload(), JsonTypeImplBmeHeater.class);

                        if (bmerawdata == null) {

                            // use the timestamp coming in as reference
                            bmerawdataFileDate = new Date(); // let the first profile be the reference date for the file

                            bmerawdata = new JsonTypeImplBmerawdata();
                            bmerawdataName = String.format("%s_Board_%s_PowerOnOff_1_%s_File_0.bmerawdata",
                                        BMERAWDATA_FILE_NAME_FORMAT.format(bmerawdataFileDate),
                                        bmerawdata.rawDataHeader.boardId,
                                        bmerawdata.rawDataHeader.seedPowerOnOff);

                            bmerawdata.configHeader.dateCreated = BMERAWDATA_CONFIG_HEADER_FORMAT.format(bmerawdataFileDate);
                            bmerawdata.rawDataHeader.dateCreated = String.valueOf(bmerawdataFileDate.getTime() / 1000);
                            bmerawdata.rawDataHeader.dateCreated_ISO = BMERAWDATA_DATA_HEADER_FORMAT.format(bmerawdataFileDate);

                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Sensor Index", "", "integer", "sensor_index"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Sensor ID", "", "integer", "sensor_id"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Time Since PowerOn", "Milliseconds", "integer", "timestamp_since_poweron"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Real time clock", "Unix Timestamp: seconds since Jan 01 1970. (UTC); 0 = missing", "integer", "real_time_clock"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Temperature", "DegreesCelcius", "float", "temperature"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Pressure", "Hectopascals", "float", "pressure"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Relative Humidity", "Percent", "float", "relative_humidity"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Resistance Gassensor", "Ohms", "float", "resistance_gassensor"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Heater Profile Step Index", "", "integer", "heater_profile_step_index"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Scanning Mode Enabled", "", "integer", "scanning_mode_enabled"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Label Tag", "", "integer", "label_tag"));
                            bmerawdata.rawDataBody.dataColumns.add(createDataColumn("Error Code", "", "integer", "error_code"));

                        } else {

                            // not the first profile to come in, limit incoming sensor data to the time range of the first series
                            timeSincePowerOnMax = bmerawdata.rawDataBody.dataBlock.stream().mapToLong(a -> a[2].longValue()).max().orElse(Long.MAX_VALUE);
                            System.out.println("follow up data, timeSincePowerOnMax: " + timeSincePowerOnMax);

                        }

                        refTimeSincePowerOn = bmeHeaterMessage.timeSincePowerOn;
                        refSensorId = bmeHeaterMessage.sensorId;
                        refSensorIndex = 0; // bmeHeaterMessage.sensorIndex;

                        // config body - heater profile
                        JsonTypeImplBmeHeaterProfile heaterProfileJo = new JsonTypeImplBmeHeaterProfile();
                        heaterProfileJo.id = bmeHeaterMessage.profileId;
                        for (int i = 0; i < 10; i++) {
                            heaterProfileJo.temperatureTimeVectors.add(new int[] { bmeHeaterMessage.steps.get(i * 2), bmeHeaterMessage.steps.get(i * 2 + 1) });
                        }
                        excludeForRepeatedHeaterProfileViolation = bmerawdata.configBody.heaterProfiles.stream().anyMatch(p -> p.id.equals(heaterProfileJo.id));
                        if (!excludeForRepeatedHeaterProfileViolation) {
                            bmerawdata.configBody.heaterProfiles.add(heaterProfileJo);
                        }

                        // config body - duty cycle profile
                        JsonTypeImplBmeDutyCycleProfile dutyCycleProfileJo = new JsonTypeImplBmeDutyCycleProfile();
                        boolean dutyCycleProfilePresent = bmerawdata.configBody.dutyCycleProfiles.stream().anyMatch(p -> p.id.equals(dutyCycleProfileJo.id));
                        if (!dutyCycleProfilePresent) {
                            bmerawdata.configBody.dutyCycleProfiles.add(dutyCycleProfileJo);
                        }

                        // config body - sensor configuration
                        JsonTypeImplBmeSensorConfiguration sensorConfigurationJo = new JsonTypeImplBmeSensorConfiguration();
                        sensorConfigurationJo.sensorIndex = refSensorIndex;
                        sensorConfigurationJo.heaterProfile = heaterProfileJo.id;
                        sensorConfigurationJo.dutyCycleProfile = dutyCycleProfileJo.id;
                        boolean sensorProfilePresent = bmerawdata.configBody.sensorConfigurations.stream().anyMatch(p -> p.sensorIndex == sensorConfigurationJo.sensorIndex);
                        if (!sensorProfilePresent) {
                            bmerawdata.configBody.sensorConfigurations.add(sensorConfigurationJo);
                        }

                        persistBmerawdata();

                    } else if (topic.equals("bmevalues")) {

                        JsonTypeImplBmeValues bmeValuesMessage = new ObjectMapper().readValue(message.getPayload(), JsonTypeImplBmeValues.class);

                        long valTimeSincePowerOn = bmeValuesMessage.timeSincePowerOn - refTimeSincePowerOn;
                        long unixTimestamp = (bmerawdataFileDate.getTime() + valTimeSincePowerOn) / 1000;

                        excludeForTimeSincePowerOnViolation = valTimeSincePowerOn > timeSincePowerOnMax;
                        System.out.println(
                                    "received values, excludeForRepeatedHeaterProfile: " + excludeForRepeatedHeaterProfileViolation + ", excludeForTimeSincePowerOnViolation: " + excludeForTimeSincePowerOnViolation);

                        if (!(excludeForRepeatedHeaterProfileViolation || excludeForTimeSincePowerOnViolation)) {

                            if (bmerawdata != null) {
                                bmerawdata.rawDataBody.dataBlock.add(new Number[] {
                                            refSensorIndex,
                                            refSensorId,
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
                                if (bmerawdata.rawDataBody.dataBlock.size() % 10 == 0) {
                                    persistBmerawdata();
                                }
                            }

                        }

                    }

                } catch (Exception ex) {
                    ex.printStackTrace();
                }

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                System.out.println("deliveryComplete---------" + token.isComplete());
            }

        });

        connect(client, options);

    }

    public static void persistBmerawdata() throws JsonGenerationException, JsonMappingException, IOException {

        // sort sensor configs by sensor id
        bmerawdata.configBody.sensorConfigurations.sort((a, b) -> {
            return a.sensorIndex - b.sensorIndex;
        });

        // sort all values, regardless of sensor id by timeSincePowerOn
        bmerawdata.rawDataBody.dataBlock.sort((a, b) -> {
            return a[2].intValue() - b[2].intValue();
        });

        ObjectMapper mapper = new ObjectMapper();
        DefaultPrettyPrinter prettyPrinter = new DefaultPrettyPrinter();
        prettyPrinter.indentArraysWith(DefaultIndenter.SYSTEM_LINEFEED_INSTANCE);
        mapper.setDefaultPrettyPrinter(prettyPrinter);
        mapper.writerWithDefaultPrettyPrinter().writeValue(new File("C:\\BME-Ai-Studio\\data", bmerawdataName), bmerawdata);

    }

    public static JsonTypeImplBmeDataColumn createDataColumn(String name, String unit, String format, String key) {

        JsonTypeImplBmeDataColumn dataColumnJo = new JsonTypeImplBmeDataColumn();
        dataColumnJo.name = name;
        dataColumnJo.unit = unit;
        dataColumnJo.format = format;
        dataColumnJo.key = key;
        return dataColumnJo;

    }

    public static void connect(MqttClient client, MqttConnectOptions options) throws MqttSecurityException, MqttException {
        client.connect(options);
        client.subscribe("bmeheater");
        client.subscribe("bmevalues");
        System.out.println("connected");
    }

}
