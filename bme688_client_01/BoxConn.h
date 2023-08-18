#ifndef BoxConn_h
#define BoxConn_h

#include <Arduino.h>
#include "WiFi.h"
#include "BoxCommons.h"
#include "BoxConf.h"

class BoxConn {

private:
  
public:
  static void handleStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  static void begin();
  static init_state_t initState;

};

#endif