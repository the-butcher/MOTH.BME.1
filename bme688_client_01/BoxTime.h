#ifndef BoxTime_h
#define BoxTime_h

#include "RTClib.h"
#include <ArduinoJson.h>
#include "BoxCommons.h"

class BoxTime {
  
  private:
    static void handleNtpUpdate(struct timeval *t);

  public:
    static init_state_t initState;
    static void begin();
    static void handleWifiConnected();
    static String getUptimeString(uint64_t uptimeMillis);
    static String getTimeString();

};

#endif