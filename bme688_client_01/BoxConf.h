#ifndef BoxConf_h
#define BoxConf_h

#include "LittleFS.h"
#include "FS.h"

#include <Arduino.h>
#include <ArduinoJson.h>

const String CONFIG_MQTT_HOST = "<your-mqtt-ip>";
const int CONFIG_MQTT_PORT = 1883;
const String CONFIG_WIFI_SSID = "<your-wifi-ssid>";
const String CONFIG_WIFI_PASS = "<your-wifi-password>";
const String CONFIG_TIME_ZONE = "CET-1CEST,M3.5.0,M10.5.0/3";

class BoxConf
{

private:
  static String getFileName(String name, String type);

public:
  static int bsecConfigLength;
  static String nextProfileName(String profileName);
  static void saveProfile(String name, JsonObject &profileJo);
  static JsonObject &loadProfile(String name);
  static void saveBsecConfig(String name, byte *data);
  static void loadBsecConfig(String name, uint8_t *data);
};

#endif