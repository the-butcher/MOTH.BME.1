package com.igorion.mqtt.types.message;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for a MQTT values message<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeValues extends AJsonTypeImpl {

    @JsonProperty("tpo")
    public int timeSincePowerOn;

    @JsonProperty("deg")
    public double temperature;

    @JsonProperty("hpa")
    public double pressure;

    @JsonProperty("hum")
    public double humidity;

    @JsonProperty("gas")
    public double gasResistance;

    @JsonProperty("hix")
    public int heaterStepIndex;

    @JsonProperty("six")
    public int specimenIndex;

}
