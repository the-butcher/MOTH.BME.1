#ifndef Measurements_h
#define Measurements_h

#include <Arduino.h>
#include "ValuesColl.h"

class Measurements {
  
  private:
    static int memBufferSize; // count of measurements kept in PSRAM
    static ValuesColl* measurements;
    
  public:
    static int memBufferIndx;
    static void begin();
    static void putMeasurement(ValuesColl measurement);
    static ValuesColl getOffsetMeasurement(int offset);
    static ValuesColl getLatestMeasurement();
    static ValuesColl getMeasurement(int memIndex);
    static int getFirstPublishableIndex();
    static void setPublished(int memIndex);
    static int getPublishableCount();

};

#endif