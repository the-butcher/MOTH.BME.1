#include "BoxSensMeasure.h"
#include "BoxConf.h"
#include <ArduinoJson.h>

#define NEW_GAS_MEAS (BME68X_GASM_VALID_MSK | BME68X_HEAT_STAB_MSK | BME68X_NEW_DATA_MSK)

Bsec2 BoxSensMeasure::baseSensor;
init_state_t BoxSensMeasure::initState = INIT_STATE_PENDING;
String BoxSensMeasure::profileNameCurr = "";
String BoxSensMeasure::profileNameNext = "";
ValuesMeas BoxSensMeasure::values = {
  "",
  0,
  "",
  0,
  "",
  0,
  "",
  0,
  0
};

void BoxSensMeasure::begin() {

  // TODO :: error handling
  JsonObject& profileJo = BoxConf::loadProfile(BoxSensMeasure::profileNameCurr);
  BoxSensMeasure::values.gasNameA = profileJo.get<String>("gasName1");
  BoxSensMeasure::values.gasNameB = profileJo.get<String>("gasName2");
  BoxSensMeasure::values.gasNameC = profileJo.get<String>("gasName3");
  BoxSensMeasure::values.gasNameD = profileJo.get<String>("gasName4");

  // TODO :: error handling
  uint8_t bsecConfig[BoxConf::bsecConfigLength];
  BoxConf::loadBsecConfig(BoxSensMeasure::profileNameCurr, bsecConfig);

  bsecSensor sensorList[] = {
    BSEC_OUTPUT_GAS_ESTIMATE_1,
    BSEC_OUTPUT_GAS_ESTIMATE_2,
    BSEC_OUTPUT_GAS_ESTIMATE_3,
    BSEC_OUTPUT_GAS_ESTIMATE_4
  };

  if (!BoxSensMeasure::baseSensor.begin(0x77, Wire)) {
    checkBsecStatus();
  }

  if (!BoxSensMeasure::baseSensor.setConfig(bsecConfig)) {
    checkBsecStatus();
  }

  if (!BoxSensMeasure::baseSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_SCAN)) {
    checkBsecStatus();
  }

  if (!BoxSensMeasure::baseSensor.run()) {
    checkBsecStatus();
  }

  BoxSensMeasure::baseSensor.attachCallback(BoxSensMeasure::newDataCallback);

  // Serial.println("BSEC library version " + String(BoxSensMeasure::baseSensor.version.major) + "."
  //                + String(BoxSensMeasure::baseSensor.version.minor) + "."
  //                + String(BoxSensMeasure::baseSensor.version.major_bugfix) + "."
  //                + String(BoxSensMeasure::baseSensor.version.minor_bugfix));

  BoxSensMeasure::initState = INIT_STATE_SUCCESS;

}

void BoxSensMeasure::newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec) {

  for (uint8_t i = 0; i < outputs.nOutputs; i++) {

    const bsecData output = outputs.output[i];
    switch (output.sensor_id) {

      case BSEC_OUTPUT_GAS_ESTIMATE_1:
        BoxSensMeasure::values.probabilityA = output.signal * 100;
        BoxSensMeasure::values.accuracy = max(BoxSensMeasure::values.accuracy, output.accuracy);
        break;
      case BSEC_OUTPUT_GAS_ESTIMATE_2:
        BoxSensMeasure::values.probabilityB = output.signal * 100;
        BoxSensMeasure::values.accuracy = max(BoxSensMeasure::values.accuracy, output.accuracy);
        break;      
      case BSEC_OUTPUT_GAS_ESTIMATE_3:
        BoxSensMeasure::values.probabilityC = output.signal * 100;
        BoxSensMeasure::values.accuracy = max(BoxSensMeasure::values.accuracy, output.accuracy);
        break;      
      case BSEC_OUTPUT_GAS_ESTIMATE_4:
        BoxSensMeasure::values.probabilityD = output.signal * 100;
        BoxSensMeasure::values.accuracy = max(BoxSensMeasure::values.accuracy, output.accuracy);
        break;
      default:
        break;

    }
  }

}

void BoxSensMeasure::checkBsecStatus() {

  if (BoxSensMeasure::baseSensor.status < BSEC_OK) {
    Serial.println("BSEC error code : " + String(BoxSensMeasure::baseSensor.status));
  } else if (BoxSensMeasure::baseSensor.status > BSEC_OK) {
    Serial.println("BSEC warning code : " + String(BoxSensMeasure::baseSensor.status));
  }

  if (BoxSensMeasure::baseSensor.sensor.status < BME68X_OK) {
    Serial.println("BME68X error code : " + String(BoxSensMeasure::baseSensor.sensor.status));
  } else if (BoxSensMeasure::baseSensor.sensor.status > BME68X_OK) {
    Serial.println("BME68X warning code : " + String(BoxSensMeasure::baseSensor.sensor.status));
  }

}

bool BoxSensMeasure::tryRead() {
  
  if (!BoxSensMeasure::baseSensor.run()) {
    checkBsecStatus();
  }

  // TODO this should return true once after new data callback (?)

  return false;

}

String getGasName(int gasIndex) {
  return "G";
}

void BoxSensMeasure::nextProfileName() {
  BoxSensMeasure::profileNameCurr = BoxSensMeasure::profileNameNext;
  BoxSensMeasure::profileNameNext = BoxConf::nextProfileName(BoxSensMeasure::profileNameCurr);
}

String BoxSensMeasure::getProfileNameCurr() {
  return BoxSensMeasure::profileNameCurr;
}

String BoxSensMeasure::getProfileNameNext() {
  return BoxSensMeasure::profileNameNext;
}
