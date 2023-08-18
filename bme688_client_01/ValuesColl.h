#ifndef ValuesColl_h
#define ValuesColl_h

typedef struct {
  uint64_t timeSincePowerOn;
  float temperature;
  float pressure;
  float humidity;
  float gasResistance;
  int heaterStepIndex;
  int specimenIndex;
  bool publishable;
} ValuesColl;

#endif