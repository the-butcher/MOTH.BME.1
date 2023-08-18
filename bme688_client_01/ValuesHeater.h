#ifndef HeaterProfile_h
#define HeaterProfile_h

typedef struct {
  int deg;
  int num;
} HeaterProfileStep;

typedef struct {
  String id;
  int sensorIndex;
  uint64_t sensorId;
  HeaterProfileStep steps[10];
} HeaterProfile;

const HeaterProfile HEATER_301 = {
  "heater_301",
  0,
  1000000000,
  {
    {100, 2},
    {100, 41},
    {200, 2},
    {200, 14},
    {200, 14},
    {200, 14},
    {320, 2},
    {320, 14},
    {320, 14},
    {320, 14}
  }
};

const HeaterProfile HEATER_354 = {
  "heater_354",
  1,
  1000000001,
  {
    {320, 5},
    {100, 2},
    {100, 10},
    {100, 30},
    {200, 5},
    {200, 5},
    {200, 5},
    {320, 5},
    {320, 5},
    {320, 5}
  }
};

const HeaterProfile HEATER_411 = {
  "heater_411",
  2,
  1000000002,
  {
    {100,43},
    {320,2},
    {170,43},
    {320,2},
    {240,2},
    {240,20},
    {240,21},
    {320,2},
    {320,20},
    {320,21}
  }
};

const HeaterProfile HEATER_501 = {
  "heater_501",
  3,
  1000000003,
  {
    {210,24},
    {265,2},
    {265,22},
    {320,2},
    {320,22},
    {265,24},
    {210,24},
    {155,24},
    {100,24},
    {155,24}
  }
};


#endif