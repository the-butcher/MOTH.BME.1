#include <stdint.h>
#include "BoxTime.h"

#include "time.h"
#include "sntp.h"
#include "BoxMqtt.h"
#include "BoxConf.h"

#define MILLISECONDS_PER_HOUR UINT32_C(60 * 60 * 1000) // 1 HOUR

init_state_t BoxTime::initState = INIT_STATE_PENDING;

void BoxTime::begin() {

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: optNtpUpdate() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  sntp_servermode_dhcp(1);    // (optional)

  // // set notification call-back function 
  sntp_set_time_sync_notification_cb(BoxTime::handleNtpUpdate);

}

void BoxTime::handleWifiConnected() {
  configTzTime(CONFIG_TIME_ZONE.c_str(), "pool.ntp.org", "time.nist.gov");
}

// Callback function (get's called when time adjusts via NTP)
void BoxTime::handleNtpUpdate(struct timeval *t) {
  BoxTime::initState = INIT_STATE_SUCCESS;
}

String BoxTime::getUptimeString(uint64_t uptimeMillis) {

  // uint64_t _millis = millis() / 1000;

  int seconds = (uptimeMillis / 1000) % 60;
  int minutes = (uptimeMillis / 60000) % 60;
  int hours = (uptimeMillis / 3600000) % 24;

  char uptimeBuffer[16];
  sprintf(uptimeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);
  return uptimeBuffer;


}

String BoxTime::getTimeString() {

  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeBuffer[16];
  sprintf(timeBuffer, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return timeBuffer;
  
}

