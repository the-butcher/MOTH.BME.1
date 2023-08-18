#include "esp_wifi_types.h"
#include "WiFiType.h"
#include "esp32-hal.h"

#include "BoxConn.h"
#include "BoxTime.h"
#include "BoxMqtt.h"
#include <ArduinoJson.h>

init_state_t BoxConn::initState = INIT_STATE_PENDING;

void BoxConn::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(CONFIG_WIFI_SSID.c_str(), CONFIG_WIFI_PASS.c_str());
  WiFi.onEvent(BoxConn::handleStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
}

void BoxConn::handleStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  BoxConn::initState = INIT_STATE_SUCCESS;
}
