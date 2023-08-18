#ifndef BoxDisplay_h
#define BoxDisplay_h

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BoxCommons.h"

class BoxDisplay { 
  
  private:
    static Adafruit_SSD1306 baseDisplay;
    static void printState(init_state_t state);
    static void printTime();
    static String formatString(String value, char const *format);
    
  public:
    static void begin();
    static void renderRoot();
    static void renderInit();
    static void renderMenuCollect();
    static void renderDataCollect();
    static void renderMenuMeasure();
    static void renderDataMeasure();

};

#endif