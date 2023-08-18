package com.igorion.mqtt.types.bmerawdata;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the bmerawdata format as in Bosch AI-Studio documentation<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmerawdata extends AJsonTypeImpl {

    @JsonProperty("configHeader")
    public JsonTypeImplBmeConfigHeader configHeader = new JsonTypeImplBmeConfigHeader();

    @JsonProperty("configBody")
    public JsonTypeImplBmeConfigBody configBody = new JsonTypeImplBmeConfigBody();

    @JsonProperty("rawDataHeader")
    public JsonTypeImplBmeRawDataHeader rawDataHeader = new JsonTypeImplBmeRawDataHeader();

    @JsonProperty("rawDataBody")
    public JsonTypeImplBmeRawDataBody rawDataBody = new JsonTypeImplBmeRawDataBody();

}
