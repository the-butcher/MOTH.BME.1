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
public class JsonTypeImplBmeDutyCycleProfile extends AJsonTypeImpl {

    @JsonProperty("id")
    public String id = "duty_1";

    @JsonProperty("numberScanningCycles")
    public int numberScanningCycles = 1;

    @JsonProperty("numberSleepingCycles")
    public int numberSleepingCycles = 0;

}
