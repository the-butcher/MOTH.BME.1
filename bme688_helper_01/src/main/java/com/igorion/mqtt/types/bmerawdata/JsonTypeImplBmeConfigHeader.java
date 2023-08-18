package com.igorion.mqtt.types.bmerawdata;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.igorion.type.json.impl.AJsonTypeImpl;

/**
 * json mapping for the ConfigHeader section of bmerawdata<br>
 *
 * @author h.fleischer
 * @since 12.08.2023
 *
 */
@JsonIgnoreProperties(ignoreUnknown = true)
@JsonInclude(Include.NON_NULL)
public class JsonTypeImplBmeConfigHeader extends AJsonTypeImpl {

    @JsonProperty("dateCreated")
    public String dateCreated;

    @JsonProperty("appVersion")
    public String appVersion = "1.6.0";

    @JsonProperty("boardType")
    public String boardType = "board_x";

    @JsonProperty("boardMode")
    public String boardMode = "heater_profile_exploration";

    @JsonProperty("boardLayout")
    public String boardLayout = "single";

}
