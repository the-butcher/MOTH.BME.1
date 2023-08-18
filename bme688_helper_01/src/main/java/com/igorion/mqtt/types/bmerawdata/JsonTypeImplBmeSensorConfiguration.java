package com.igorion.mqtt.types.bmerawdata;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the ConfigBody/DutyCycleProfile section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeSensorConfiguration extends AJsonTypeImpl {

    @JsonProperty("sensorIndex")
    public int sensorIndex;

    @JsonProperty("heaterProfile")
    public String heaterProfile;

    @JsonProperty("dutyCycleProfile")
    public String dutyCycleProfile;

}
