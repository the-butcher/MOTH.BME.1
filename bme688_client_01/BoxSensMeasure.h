#ifndef BoxSensMeasure_h
#define BoxSensMeasure_h

#include <Wire.h>
#include <bsec2.h>
#include "ValuesMeas.h"
#include "BoxCommons.h"

class BoxSensMeasure {
  
  private:
    static Bsec2 baseSensor;
    static String profileNameCurr;
    static String profileNameNext;
    static void checkBsecStatus();
    static void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

  public:
    static init_state_t initState;
    static void begin();
    static bool tryRead();
    static ValuesMeas values;
    static void nextProfileName();
    static String getProfileNameCurr();
    static String getProfileNameNext();
    static String getGasName(int gasIndex);

};

#endif