package com.igorion.mqtt.type;

import java.util.List;

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
public class JsonTypeImplBsecProfile extends AJsonTypeImpl {

    @JsonProperty("name")
    public String name;

    @JsonProperty("gasName1")
    public String gasName1;

    @JsonProperty("gasName2")
    public String gasName2;

    @JsonProperty("gasName3")
    public String gasName3;

    @JsonProperty("gasName4")
    public String gasName4;

    @JsonProperty("data")
    public List<Integer> data;

}
