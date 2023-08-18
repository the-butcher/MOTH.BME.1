package com.igorion.mqtt.type;

import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the rawDataBody section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeRawDataBody extends AJsonTypeImpl {

    @JsonProperty("dataColumns")
    public List<JsonTypeImplBmeDataColumn> dataColumns = new ArrayList<>();

    @JsonProperty("dataBlock")
    public List<Number[]> dataBlock = new ArrayList<>();

}
