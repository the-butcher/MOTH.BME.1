#include "LittleFS.h"
#include "FS.h"
#include "BoxMqtt.h"
#include <WiFiClient.h>
#include "BoxConn.h"
#include "BoxConf.h"
#include "Measurements.h"
#include <ArduinoJson.h>

WiFiClient *wifiClient;
PubSubClient *mqttClient;

int BoxMqtt::failureCount = 0;
int BoxMqtt::mqttState;
String BoxMqtt::profileName = "";

init_state_t BoxMqtt::initState = INIT_STATE_PENDING;

void BoxMqtt::begin() {
  // do nothing
}

void BoxMqtt::handleWifiConnected() {

  BoxMqtt::checkClients();
  bool connected = BoxMqtt::checkConnect();
  if (connected) {
    BoxMqtt::initState = INIT_STATE_SUCCESS;
  } else {
    BoxMqtt::initState = INIT_STATE_FAILURE;
  }

}

bool BoxMqtt::loop() {
  if (mqttClient != NULL && mqttClient->connected()) {
    return mqttClient->loop();
  } else {
    return false;
  }
}

void BoxMqtt::checkClients() {

  if (wifiClient == NULL || mqttClient == NULL) {
    wifiClient = new WiFiClient();
    mqttClient = new PubSubClient(*wifiClient);
    mqttClient->setServer(CONFIG_MQTT_HOST.c_str(), CONFIG_MQTT_PORT);
    mqttClient->setCallback(BoxMqtt::callback);
    mqttClient->disconnect();
  }

}

bool BoxMqtt::checkConnect() {

  if (!mqttClient->connected()) {
    mqttClient->connect("ESP32Client");  // connect without credentials
  } 

  // must be connected to subscribe
  if (mqttClient->connected()) {
    mqttClient->subscribe("bsecDefs");
    // mqttClient->subscribe("bsecData");
  }

  return mqttClient->connected();

}

void BoxMqtt::callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("callback: ");
  Serial.print(topic);
  Serial.print(", length: ");
  Serial.println(length);

  if (String(topic) == "bsecDefs" && length > 0) {

    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& profileJo = jsonBuffer.parseObject((char *)payload);
    BoxMqtt::profileName = profileJo.get<String>("name");
    Serial.println(BoxMqtt::profileName);

    BoxConf::saveProfile(BoxMqtt::profileName, profileJo);

    mqttClient->unsubscribe("bsecData");
    mqttClient->subscribe("bsecData");

  } else if (String(topic) == "bsecData" && length > 0) {

    BoxConf::saveBsecConfig(BoxMqtt::profileName, payload);

    mqttClient->subscribe("bsecData");
    mqttClient->unsubscribe("bsecData");

  }

}

bool BoxMqtt::hasFailure() {
  return failureCount >= 5;
}

void BoxMqtt::publishProfile(HeaterProfile profile) {

  BoxMqtt::mqttState = MQTT_STATUS___________________OK;
  if (WiFi.status() == WL_CONNECTED) {

    // be sure there are valid wifi and mqtt clients
    BoxMqtt::checkClients();
    bool connected = BoxMqtt::checkConnect();
    if (connected) {

      uint64_t timeSincePowerOn = millis();

      DynamicJsonBuffer jsonBuffer;
      JsonObject &rootJson = jsonBuffer.createObject();
      rootJson["pid"] = profile.id;
      rootJson["tpo"] = String(timeSincePowerOn);
      rootJson["six"] = String(profile.sensorIndex);
      rootJson["sid"] = String(profile.sensorId);      
      JsonArray &stepsJa = rootJson.createNestedArray("stp");
      for (int i = 0; i < 10; i++) {
        stepsJa.add(profile.steps[i].deg);
        stepsJa.add(profile.steps[i].num);
      }

      if (!publishJson(rootJson, "bmeheater")) {

        Serial.println("mqtt publish profile failed");
        BoxMqtt::mqttState = MQTT_STATUS_FAILURE______PUBLISH;

      }

    } else {  // not connected

      Serial.println("mqtt connection lost");
      BoxMqtt::handleFailure(mqttClient->state());

    }

  } else if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Wifi connection lost");
    BoxMqtt::mqttState = MQTT_STATUS_INVALID__WIFI_STATUS;
    
  } else {
    BoxMqtt::mqttState = MQTT_STATUS______________UNKNOWN;
  }

  if (BoxMqtt::mqttState != MQTT_STATUS___________________OK) {
    failureCount++;
  }

}

void BoxMqtt::publishValues() {

  BoxMqtt::mqttState = MQTT_STATUS___________________OK;
  if (WiFi.status() == WL_CONNECTED) {

    // be sure there are valid wifi and mqtt clients
    BoxMqtt::checkClients();
    bool connected = BoxMqtt::checkConnect();
    if (connected) {

      int firstPublishableIndex;
      int numPublished = 0;
      while ((firstPublishableIndex = Measurements::getFirstPublishableIndex()) >= 0) {

        ValuesColl measurement = Measurements::getMeasurement(firstPublishableIndex);

        DynamicJsonBuffer jsonBuffer;
        JsonObject &rootJson = jsonBuffer.createObject();
        rootJson["tpo"] = String(measurement.timeSincePowerOn);
        rootJson["deg"] = String(measurement.temperature, 2);
        rootJson["hpa"] = String(measurement.pressure, 2);
        rootJson["hum"] = String(measurement.humidity, 2);
        rootJson["gas"] = String(measurement.gasResistance, 2);
        rootJson["hix"] = String(measurement.heaterStepIndex);
        rootJson["six"] = String(measurement.specimenIndex);

        if (publishJson(rootJson, "bmevalues")) {

          Measurements::setPublished(firstPublishableIndex);

        } else {

          Serial.println("mqtt publish failed");
          BoxMqtt::mqttState = MQTT_STATUS_FAILURE______PUBLISH;
          break;

        }

      }

    } else {  // not connected

      Serial.println("mqtt connection lost");
      BoxMqtt::handleFailure(mqttClient->state());

    }
    
  } else if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Wifi connection lost");
    BoxMqtt::mqttState = MQTT_STATUS_INVALID__WIFI_STATUS;

  } else {
    BoxMqtt::mqttState = MQTT_STATUS______________UNKNOWN;
  }

  if (BoxMqtt::mqttState != MQTT_STATUS___________________OK) {
    failureCount++;
  }

}

void BoxMqtt::handleFailure(int state) {
  if (state == MQTT_CONNECTION_TIMEOUT) {
    BoxMqtt::mqttState = MQTT_STATUS_TIMEOUT___CONNECTION;
  } else if (state == MQTT_CONNECTION_LOST) {
    BoxMqtt::mqttState = MQTT_STATUS_LOST______CONNECTION;
  } else if (state == MQTT_CONNECT_FAILED) {
    BoxMqtt::mqttState = MQTT_STATUS_FAILURE___CONNECTION;
  } else if (state == MQTT_DISCONNECTED) {
    BoxMqtt::mqttState = MQTT_STATUS_LOST______CONNECTION;
  } else if (state == MQTT_CONNECT_BAD_PROTOCOL) {
    BoxMqtt::mqttState = MQTT_STATUS_INVALID_____PROTOCOL;
  } else if (state == MQTT_CONNECT_BAD_CLIENT_ID) {
    BoxMqtt::mqttState = MQTT_STATUS_INVALID_____CLIENTID;
  } else if (state == MQTT_CONNECT_UNAVAILABLE) {
    BoxMqtt::mqttState = MQTT_STATUS_UNAVAIL___CONNECTION;
  } else if (state == MQTT_CONNECT_BAD_CREDENTIALS) {
    BoxMqtt::mqttState = MQTT_STATUS_INVALID__CREDENTIALS;
  } else if (state == MQTT_CONNECT_UNAUTHORIZED) {
    BoxMqtt::mqttState = MQTT_STATUS_INVALID_UNAUTHORIZED;
  } else {
    BoxMqtt::mqttState = MQTT_STATUS______________UNKNOWN;
  }
}

bool BoxMqtt::publishJson(JsonObject &root, char *mqid) {

  String outputStr;
  root.printTo(outputStr);
  int outputLen = outputStr.length() + 1;

  char outputBuf[outputLen];
  outputStr.toCharArray(outputBuf, outputLen);

  return mqttClient->publish(mqid, (uint8_t const *)outputBuf, outputLen);
}