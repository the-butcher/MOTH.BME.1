package com.igorion.mqtt.type;

import java.util.Date;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for an arcgis server feature attributes object<br>
 *
 * @author h.fleischer
 * @since 14.03.2020
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplMessage extends AJsonTypeImpl {

    @JsonProperty("time")
    public Date time;

    @JsonProperty("client")
    public String client;

    @JsonProperty("co2")
    public Integer co2;

    @JsonProperty("temperature")
    public Double temperature;

    @JsonProperty("humidity")
    public Double humidity;

    @JsonProperty("pressure")
    public Double pressure;

    @JsonProperty("percent")
    public Double percent;

    @JsonProperty("voltage")
    public Double voltage;

}
