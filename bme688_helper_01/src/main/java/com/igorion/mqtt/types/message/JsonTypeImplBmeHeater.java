package com.igorion.mqtt.types.message;

import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the heater profile mqtt message<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeHeater extends AJsonTypeImpl {

    @JsonProperty("tpo")
    public int timeSincePowerOn;

    @JsonProperty("pid")
    public String profileId;

    @JsonProperty("six")
    public int sensorIndex;

    @JsonProperty("sid")
    public int sensorId;

    @JsonProperty("stp")
    public List<Integer> steps = new ArrayList<>();

}
