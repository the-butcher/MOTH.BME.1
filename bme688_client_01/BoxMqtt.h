#ifndef BoxMqtt_h
#define BoxMqtt_h 

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "BoxCommons.h"
#include "ValuesHeater.h"

#define MQTT_STATUS___________________OK 10
#define MQTT_STATUS_FAILURE______PUBLISH 11
#define MQTT_STATUS_FAILURE___CONNECTION 12
#define MQTT_STATUS_TIMEOUT___CONNECTION 13
#define MQTT_STATUS_LOST______CONNECTION 14
#define MQTT_STATUS_UNAVAIL___CONNECTION 15
#define MQTT_STATUS_INVALID_____PROTOCOL 16
#define MQTT_STATUS_INVALID_____CLIENTID 17
#define MQTT_STATUS_INVALID__CREDENTIALS 18
#define MQTT_STATUS_INVALID_UNAUTHORIZED 19
#define MQTT_STATUS_INVALID____WIFI_MODE 20 // not used in this project
#define MQTT_STATUS_INVALID__WIFI_STATUS 21
#define MQTT_STATUS_INVALID_________ADDR 22
#define MQTT_STATUS_INVALID_________PORT 23
#define MQTT_STATUS______________UNKNOWN 24

class BoxMqtt {
  
  private:
    static int failureCount;
    static String profileName;
    static bool publishJson(JsonObject &root, char* mqid);
    static void checkClients();
    static bool checkConnect();
    static void callback(char* topic, byte* payload, unsigned int length);
    static void handleFailure(int state);
    static String createRandomName();
    
  public:
    static init_state_t initState;
    static int mqttState;
    static void begin();
    static bool loop();
    static void publishValues();
    static void publishProfile(HeaterProfile profile);
    static bool hasFailure();
    static void handleWifiConnected();

};

#endif