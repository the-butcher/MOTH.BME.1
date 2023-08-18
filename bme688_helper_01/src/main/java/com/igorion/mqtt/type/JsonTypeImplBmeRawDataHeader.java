package com.igorion.mqtt.type;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the rawDataHeader section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeRawDataHeader extends AJsonTypeImpl {

    @JsonProperty("counterPowerOnOff")
    public int counterPowerOnOff = 1;

    @JsonProperty("seedPowerOnOff")
    public String seedPowerOnOff = "jecxzq530rhj2r5x";

    @JsonProperty("counterFileLimit")
    public int counterFileLimit = 1;

    @JsonProperty("dateCreated")
    public String dateCreated;

    @JsonProperty("dateCreated_ISO")
    public String dateCreated_ISO;

    @JsonProperty("firmwareVersion")
    public String firmwareVersion = "1.5.0";

    @JsonProperty("boardId")
    public String boardId = "1000000000";

}
