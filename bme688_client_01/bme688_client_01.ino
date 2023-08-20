#include <Wire.h>

#include "LittleFS.h"
#include "BoxDisplay.h"
#include "BoxCommons.h"
#include "BoxConn.h"
#include "BoxTime.h"
#include "BoxMqtt.h"
#include "BoxSensCollect.h"
#include "BoxSensMeasure.h"
#include "ValuesHeater.h"
#include "Measurements.h"

const int BUTTON_PIN_A = GPIO_NUM_9;
const int BUTTON_PIN_B = GPIO_NUM_6;
const int BUTTON_PIN_C = GPIO_NUM_5;

uint64_t buttonAMillis = 0;
uint64_t buttonBMillis = 0;
uint64_t buttonCMillis = 0;

init_modus_t initModus;

void setup() {
  
  Wire.begin();
  Serial.begin(115200);
  delay(3000);
  LittleFS.begin(true);

  // Serial.println("------------------------------------");
  // File dir = LittleFS.open("/");
  // String fileName;
  // while ((fileName = dir.getNextFileName()) != "") {
  //   Serial.println(fileName);
  // }
  // // LittleFS.remove("/file.type");
  // LittleFS.remove("/air-lav.dat");
  // LittleFS.remove("/air-lav.prf");  
  // Serial.println("------------------------------------");

  pinMode(BUTTON_PIN_A, INPUT_PULLUP);
  pinMode(BUTTON_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON_PIN_C, INPUT_PULLUP);

  // display
  BoxDisplay::begin();

  int minLoopCount = 100; // 5 seconds max  
  int curLoopCount;

  BoxTime::begin();
  BoxMqtt::begin();
  BoxConn::begin(); // once wifi connects, it will trigger callback on time and mqtt

  // WiFi connection
  curLoopCount = 0;
  while (curLoopCount++ < minLoopCount) {
    BoxDisplay::renderInit();
    if (BoxConn::initState != INIT_STATE_PENDING) {
      break;
    }
    delay(100);
  }
  delay(500);

  // MQTT connection
  BoxMqtt::handleWifiConnected();
  curLoopCount = 0;
  while (curLoopCount++ < minLoopCount) {
    BoxDisplay::renderInit();
    if (BoxMqtt::initState != INIT_STATE_PENDING) {
      break;
    }
    delay(100);
  }
  delay(500);

  // RTC adjustment
  BoxTime::handleWifiConnected();
  curLoopCount = 0;
  while (curLoopCount++ < minLoopCount) {
    BoxDisplay::renderInit();
    if (BoxTime::initState != INIT_STATE_PENDING) {
      break;
    }
    delay(100);
  }

  // in case of failure stop here
  if (BoxConn::initState != INIT_STATE_SUCCESS || BoxMqtt::initState != INIT_STATE_SUCCESS || BoxTime::initState != INIT_STATE_SUCCESS) {

    while (true) {
      BoxDisplay::renderInit();
      delay(1000);
    }

  } else {

    // wait a bit before showing the modus options
    delay(1000);
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B), handleButtonBChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C), handleButtonCChange, CHANGE);
    while (true) {
      BoxDisplay::renderRoot(); // time must count
      if (recentPress(buttonBMillis, 500)) {
        initModus = INIT_MODUS_COLLECT;
        break;
      } else if (recentPress(buttonCMillis, 500)) {
        initModus = INIT_MODUS_MEASURE;
        break;
      }
      BoxMqtt::loop(); // maintain MQTT
      delay(500);
    }
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B));
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C));
    buttonBMillis = 0; // unpress the buttons
    buttonCMillis = 0; // unpress the buttons

    // collecting sensor data
    if (initModus == INIT_MODUS_COLLECT) {

      BoxSensCollect::begin();
      Measurements::begin();

      // pick a heater profile
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B), handleButtonBChange, CHANGE);
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C), handleButtonCChange, CHANGE);
      while (!recentPress(buttonCMillis, 1000)) {
        if (recentPress(buttonBMillis, 500)) {
          if (BoxSensCollect::getHeaterProfile().id == HEATER_001.id) {
            BoxSensCollect::setHeaterProfile(HEATER_301);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_301.id) {
            BoxSensCollect::setHeaterProfile(HEATER_321);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_321.id) {
            BoxSensCollect::setHeaterProfile(HEATER_322);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_322.id) {
            BoxSensCollect::setHeaterProfile(HEATER_323);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_323.id) {
            BoxSensCollect::setHeaterProfile(HEATER_324);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_324.id) {
            BoxSensCollect::setHeaterProfile(HEATER_331);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_331.id) {
            BoxSensCollect::setHeaterProfile(HEATER_332);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_332.id) {
            BoxSensCollect::setHeaterProfile(HEATER_354);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_354.id) {
            BoxSensCollect::setHeaterProfile(HEATER_411);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_411.id) {
            BoxSensCollect::setHeaterProfile(HEATER_412);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_412.id) {
            BoxSensCollect::setHeaterProfile(HEATER_413);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_413.id) {
            BoxSensCollect::setHeaterProfile(HEATER_414);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_414.id) {
            BoxSensCollect::setHeaterProfile(HEATER_501);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_501.id) {
            BoxSensCollect::setHeaterProfile(HEATER_502);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_502.id) {
            BoxSensCollect::setHeaterProfile(HEATER_503);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_503.id) {
            BoxSensCollect::setHeaterProfile(HEATER_504);
          } else if (BoxSensCollect::getHeaterProfile().id == HEATER_504.id) {
            BoxSensCollect::setHeaterProfile(HEATER_001);
          }
        }
        BoxDisplay::renderMenuCollect();
        BoxMqtt::loop(); // maintain MQTT
        delay(500);
      }
      detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B));
      detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C));
      buttonCMillis = 0; // unpress the button

      // setup for collecting
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C), handleButtonCChange, CHANGE);
      BoxMqtt::publishProfile(BoxSensCollect::getHeaterProfile());
      BoxSensCollect::startCollecting();
      xTaskCreatePinnedToCore(renderAndPublish, "renderAndPublish", 10000, NULL, 0, NULL, 0);      

    } else if (initModus == INIT_MODUS_MEASURE) {
      
      BoxSensMeasure::nextProfileName(); // moves next to curr and re-evaluates next
      BoxSensMeasure::nextProfileName(); // moves the now valid next into curr

      // pick a bsec profile
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B), handleButtonBChange, CHANGE);
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C), handleButtonCChange, CHANGE);
      while (!recentPress(buttonCMillis, 1000) || BoxSensMeasure::getProfileNameCurr() == "") { 
        if (recentPress(buttonBMillis, 500)) {
          BoxSensMeasure::nextProfileName();
        }
        BoxDisplay::renderMenuMeasure();
        BoxMqtt::loop(); // maintain MQTT
        delay(500);
      }
      detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_B));
      detachInterrupt(digitalPinToInterrupt(BUTTON_PIN_C));
      buttonCMillis = 0; // unpress the button   

      BoxSensMeasure::begin();

      xTaskCreatePinnedToCore(renderAndPublish, "renderAndPublish", 10000, NULL, 0, NULL, 0);      
      
    } else {

      // TODO :: error -> invalid INIT_MODUS

    }

  }

}

void renderAndPublish(void * parameter) {

  uint64_t currMillis = 0;
  uint64_t lastMillis = 0;

  while (true) {

    currMillis = millis();
    if (currMillis - lastMillis > 500) {

      lastMillis = currMillis;

      BoxMqtt::loop();

      if (initModus == INIT_MODUS_COLLECT) {

        BoxDisplay::renderDataCollect();
        if (Measurements::getPublishableCount() >= 10) {
          BoxMqtt::publishValues();
        }

        if (recentPress(buttonCMillis, 500)) {
          BoxSensCollect::nextSpecimenIndex();
        }

      } else if (initModus == INIT_MODUS_MEASURE) {

        BoxDisplay::renderDataMeasure();

      } else {

      // TODO :: error -> invalid INIT_MODUS

      }

    }

  }
    
  vTaskDelete(NULL);

}

bool recentPress(uint64_t _millis, uint64_t tolerance) {
  return millis() - _millis <= tolerance;
}

void handleButtonAChange() {
  if (digitalRead(BUTTON_PIN_A) == LOW) {
    buttonAMillis = millis();
  }
}

void handleButtonBChange() {
  if (digitalRead(BUTTON_PIN_B) == LOW) {
    buttonBMillis = millis();
  }
}

void handleButtonCChange() {
  if (digitalRead(BUTTON_PIN_C) == LOW) {
    buttonCMillis = millis();
  }
}

void loop() {

  if (initModus == INIT_MODUS_COLLECT) {

    if (BoxSensCollect::tryRead()) {
      Measurements::putMeasurement(BoxSensCollect::values);
    }
    delay(140 / 10); // 140 / 4

  } else if (initModus == INIT_MODUS_MEASURE) {

    BoxSensMeasure::tryRead();

  } else {

    // TODO :: error -> invalid INIT_MODUS

  }

}
