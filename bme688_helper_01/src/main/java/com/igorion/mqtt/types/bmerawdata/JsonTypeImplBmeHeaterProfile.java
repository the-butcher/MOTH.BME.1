package com.igorion.mqtt.types.bmerawdata;

import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the ConfigBody/HeaterProfile section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeHeaterProfile extends AJsonTypeImpl {

    @JsonProperty("id")
    public String id;

    @JsonProperty("timeBase")
    public int timeBase = 140;

    @JsonProperty("temperatureTimeVectors")
    public List<int[]> temperatureTimeVectors = new ArrayList<>();

}
