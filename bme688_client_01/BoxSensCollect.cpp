#include "BoxSensCollect.h"
#include "BoxTime.h"

#define HEATER_TIME_BASE 140

Bme68x BoxSensCollect::baseSensor;
init_state_t BoxSensCollect::initState = INIT_STATE_PENDING;

int BoxSensCollect::cycleIndex = 0;
int BoxSensCollect::stepIndex = -1;
int BoxSensCollect::specimenIndex = -1;

// default profile
HeaterProfile BoxSensCollect::heaterProfile = HEATER_354;
uint16_t BoxSensCollect::degProf[10];
uint16_t BoxSensCollect::dimProf[10];
uint64_t BoxSensCollect::specimenMillis = 0;
uint64_t BoxSensCollect::wakeupMillis = 0;

ValuesColl BoxSensCollect::values = {

};

void BoxSensCollect::begin() {

  BoxSensCollect::baseSensor.begin(0x77, Wire);

  if (BoxSensCollect::baseSensor.checkStatus()) {
    if (BoxSensCollect::baseSensor.checkStatus() == BME68X_ERROR) {
      BoxSensCollect::initState = INIT_STATE_FAILURE;
      Serial.println("Sensor error:" + BoxSensCollect::baseSensor.statusString());
      return;
    } else if (BoxSensCollect::baseSensor.checkStatus() == BME68X_WARNING) {
      Serial.println("Sensor Warning:" + BoxSensCollect::baseSensor.statusString());
    }
  }

  BoxSensCollect::initState = INIT_STATE_SUCCESS;

}

void BoxSensCollect::setHeaterProfile(HeaterProfile heaterProfile) {
  BoxSensCollect::heaterProfile = heaterProfile;
}
HeaterProfile BoxSensCollect::getHeaterProfile() {
  return BoxSensCollect::heaterProfile;
}

void BoxSensCollect::startCollecting() {

  for (int i = 0; i < 10; i++) {
    BoxSensCollect::degProf[i] = heaterProfile.steps[i].deg;
    BoxSensCollect::dimProf[i] = heaterProfile.steps[i].num;
  }
  BoxSensCollect::nextSpecimenIndex();

  uint16_t sharedHeatrDur = HEATER_TIME_BASE - (BoxSensCollect::baseSensor.getMeasDur(BME68X_PARALLEL_MODE) / INT64_C(1000));

  BoxSensCollect::baseSensor.setTPH();
  BoxSensCollect::baseSensor.setHeaterProf(degProf, dimProf, sharedHeatrDur, 10);
  BoxSensCollect::baseSensor.setOpMode(BME68X_PARALLEL_MODE);

}

int BoxSensCollect::getCycleIndex() {
  return BoxSensCollect::cycleIndex;
}

int BoxSensCollect::getStepIndex() {
  return BoxSensCollect::stepIndex;
}

int BoxSensCollect::getSpecimenIndex() {
  return BoxSensCollect::specimenIndex;
}

void BoxSensCollect::nextSpecimenIndex() {
  BoxSensCollect::specimenIndex = BoxSensCollect::specimenIndex + 1;
  BoxSensCollect::specimenMillis = millis();
}

 uint64_t BoxSensCollect::getSpecimenMillis() {
   return millis() - BoxSensCollect::specimenMillis;
 }

bool BoxSensCollect::tryRead() {
  
  uint8_t nFieldsLeft = 0;
  bme68xData* data;
  if (BoxSensCollect::baseSensor.getOpMode() == BME68X_SLEEP_MODE) {

    if (millis() >= BoxSensCollect::wakeupMillis) {
      BoxSensCollect::baseSensor.setOpMode(BME68X_PARALLEL_MODE);
      BoxSensCollect::cycleIndex = BoxSensCollect::cycleIndex + 1;
    }

  } else if (BoxSensCollect::baseSensor.fetchData()) {
    
    data = BoxSensCollect::baseSensor.getAllData();
    if (data->gas_index != BoxSensCollect::stepIndex) {

      BoxSensCollect::stepIndex = data->gas_index;
      // if (BoxSensCollect::stepIndex == 0) {
      //   BoxSensCollect::cycleIndex = BoxSensCollect::cycleIndex + 1;
      // }

      // upon last step put to sleep
      if (BoxSensCollect::stepIndex == 9) {
        BoxSensCollect::cycleIndex = BoxSensCollect::cycleIndex + 1;
        BoxSensCollect::baseSensor.setOpMode(BME68X_SLEEP_MODE);
        BoxSensCollect::wakeupMillis = millis() + HEATER_TIME_BASE * 0;
      }

      BoxSensCollect::values = {
        millis(),
        data->temperature,
        data->pressure,
        data->humidity,
        data->gas_resistance,
        BoxSensCollect::stepIndex,
        BoxSensCollect::specimenIndex,
        BoxSensCollect::cycleIndex >= 1, // publishable (the condition skips the first cycle)
      };    

      return true;  

    }
   
  } 
  
  return false;

}
