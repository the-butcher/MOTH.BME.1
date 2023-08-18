package com.igorion.mqtt.types.bmerawdata;

import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the ConfigBody section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeConfigBody extends AJsonTypeImpl {

    @JsonProperty("heaterProfiles")
    public List<JsonTypeImplBmeHeaterProfile> heaterProfiles = new ArrayList<>();

    @JsonProperty("dutyCycleProfiles")
    public List<JsonTypeImplBmeDutyCycleProfile> dutyCycleProfiles = new ArrayList<>();

    @JsonProperty("sensorConfigurations")
    public List<JsonTypeImplBmeSensorConfiguration> sensorConfigurations = new ArrayList<>();

}
