#ifndef BoxSensCollect_h
#define BoxSensCollect_h

#include <Wire.h>
#include <SPI.h>
#include "bme68xLibrary.h"
#include "ValuesColl.h"
#include "BoxCommons.h"
#include "ValuesHeater.h"

class BoxSensCollect {
  
  private:
    static Bme68x baseSensor;
    static int cycleIndex;
    static int stepIndex;
    static int specimenIndex; // incrementable specimen index
    static HeaterProfile heaterProfile;
    static uint16_t degProf[10];
    static uint16_t dimProf[10];
    static uint64_t specimenMillis;
    static uint64_t wakeupMillis;

  public:
    static init_state_t initState;
    static void begin();
    static int getCycleIndex();
    static int getStepIndex();
    static int getSpecimenIndex();
    static void nextSpecimenIndex();
    static uint64_t getSpecimenMillis();
    static void startCollecting();
    static bool tryRead();
    static ValuesColl values;
    static void setHeaterProfile(HeaterProfile heaterProfile);
    static HeaterProfile getHeaterProfile();

};

#endif