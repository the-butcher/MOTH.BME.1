#include "BoxDisplay.h"
#include "BoxConn.h"
#include "BoxTime.h"
#include "BoxMqtt.h"
#include "BoxSensCollect.h"
#include "BoxSensMeasure.h"
#include "Measurements.h"

Adafruit_SSD1306 BoxDisplay::baseDisplay(128, 32, &Wire);

const char FORMAT_CELL_PERCENT[] = "%7s%%";

void BoxDisplay::begin() {
  BoxDisplay::baseDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x32
  BoxDisplay::baseDisplay.clearDisplay();
  BoxDisplay::baseDisplay.display();
  BoxDisplay::baseDisplay.setTextSize(1);
  BoxDisplay::baseDisplay.setTextColor(SSD1306_WHITE);
  BoxDisplay::baseDisplay.clearDisplay();
  BoxDisplay::baseDisplay.display();
}

String BoxDisplay::formatString(String value, char const *format) {
  char padBuffer[16];
  sprintf(padBuffer, format, value);
  return padBuffer;
}

void BoxDisplay::printState(init_state_t initState) {
  if (initState == INIT_STATE_SUCCESS) {
    BoxDisplay::baseDisplay.print("OK");
  } else if (initState == INIT_STATE_FAILURE) {
    BoxDisplay::baseDisplay.print("--");
  }
}

void BoxDisplay::printTime() {
  BoxDisplay::baseDisplay.setCursor(80, 0);
  // BoxDisplay::baseDisplay.print(BoxTime::getUptimeString(millis()));  
  BoxDisplay::baseDisplay.print(BoxTime::getTimeString());  
}

void BoxDisplay::renderInit() {

  BoxDisplay::baseDisplay.clearDisplay();

  int row = 8;

  BoxDisplay::baseDisplay.setCursor(0, row);
  BoxDisplay::printState(BoxConn::initState);
  BoxDisplay::baseDisplay.setCursor(20, row);
  BoxDisplay::baseDisplay.print("wifi");
  row = row + 8;

  BoxDisplay::baseDisplay.setCursor(0, row);
  BoxDisplay::printState(BoxMqtt::initState);
  BoxDisplay::baseDisplay.setCursor(20, row);
  BoxDisplay::baseDisplay.print("mqtt");  
  row = row + 8;

  BoxDisplay::baseDisplay.setCursor(0, row);
  BoxDisplay::printState(BoxTime::initState);
  BoxDisplay::baseDisplay.setCursor(20, row);
  BoxDisplay::baseDisplay.print("time");

  BoxDisplay::printTime();

  BoxDisplay::baseDisplay.display();

}

void BoxDisplay::renderRoot() {

  BoxDisplay::baseDisplay.clearDisplay();

  BoxDisplay::baseDisplay.setCursor(0, 16);
  BoxDisplay::baseDisplay.print("B: collect data");

  BoxDisplay::baseDisplay.setCursor(0, 24);
  BoxDisplay::baseDisplay.print("C: identify gas");

  BoxDisplay::printTime();

  BoxDisplay::baseDisplay.display();

}

void BoxDisplay::renderMenuCollect() {

  BoxDisplay::baseDisplay.clearDisplay();

  BoxDisplay::baseDisplay.setCursor(0, 0);
  BoxDisplay::baseDisplay.print(BoxSensCollect::getHeaterProfile().id);

  BoxDisplay::baseDisplay.setCursor(0, 16);
  BoxDisplay::baseDisplay.print("B: pick profile");

  BoxDisplay::baseDisplay.setCursor(0, 24);
  BoxDisplay::baseDisplay.print("C: collect data");

  BoxDisplay::printTime();

  BoxDisplay::baseDisplay.display();

}

void BoxDisplay::renderMenuMeasure() {

  BoxDisplay::baseDisplay.clearDisplay();

  BoxDisplay::baseDisplay.setCursor(0, 0);
  if (BoxSensMeasure::getProfileNameCurr() != "") {
    BoxDisplay::baseDisplay.print(BoxSensMeasure::getProfileNameCurr());
  } else {
    BoxDisplay::baseDisplay.print("no profile");
  }

  if (BoxSensMeasure::getProfileNameCurr() != BoxSensMeasure::getProfileNameNext()) {
    BoxDisplay::baseDisplay.setCursor(0, 16);
    BoxDisplay::baseDisplay.print("B: pick profile");
  }

  if (BoxSensMeasure::getProfileNameCurr() != "") {
    BoxDisplay::baseDisplay.setCursor(0, 24);
    BoxDisplay::baseDisplay.print("C: identify gas");
  }

  BoxDisplay::printTime();

  BoxDisplay::baseDisplay.display();

}

void BoxDisplay::renderDataCollect() {

  BoxDisplay::baseDisplay.clearDisplay();

  BoxDisplay::baseDisplay.setCursor(0, 0);
  BoxDisplay::baseDisplay.print(BoxSensCollect::getHeaterProfile().id);
  BoxDisplay::printTime();

  BoxDisplay::baseDisplay.setCursor(0, 8);
  BoxDisplay::baseDisplay.print("mqtt");
  
  char padBufferPublishableCount[8];
  sprintf(padBufferPublishableCount, "%4s", String(Measurements::getPublishableCount()));
  BoxDisplay::baseDisplay.setCursor(104, 8);
  BoxDisplay::baseDisplay.print(padBufferPublishableCount);  

  BoxDisplay::baseDisplay.setCursor(0, 16);
  BoxDisplay::baseDisplay.print("cycle/step");

  char padBufferCycleIndex[8];
  sprintf(padBufferCycleIndex, "%3s/%s", String(BoxSensCollect::getCycleIndex()), String(BoxSensCollect::getStepIndex()));
  BoxDisplay::baseDisplay.setCursor(98, 16);
  BoxDisplay::baseDisplay.print(padBufferCycleIndex);  

  BoxDisplay::baseDisplay.drawFastHLine(62, 20, (BoxSensCollect::getStepIndex() + 1) * 3, SSD1306_WHITE);

  BoxDisplay::baseDisplay.setCursor(0, 24);
  BoxDisplay::baseDisplay.print("C: spec");

  char padBufferSpecimenIndex[8];
  sprintf(padBufferSpecimenIndex, "%4s", String(BoxSensCollect::getSpecimenIndex()));
  BoxDisplay::baseDisplay.setCursor(36, 24);
  BoxDisplay::baseDisplay.print(padBufferSpecimenIndex);  

  BoxDisplay::baseDisplay.setCursor(80, 24);
  BoxDisplay::baseDisplay.print(BoxTime::getUptimeString(BoxSensCollect::getSpecimenMillis()));  

  BoxDisplay::baseDisplay.display();

}

void BoxDisplay::renderDataMeasure() {

  BoxDisplay::baseDisplay.clearDisplay();

  int col1 = 0;
  int col2 = 28;
  int row = 16;

  if (BoxSensMeasure::values.gasNameC != "") {
    row = row - 8;
  }
  if (BoxSensMeasure::values.gasNameD != "") {
    row = row - 8;
  }

  BoxDisplay::baseDisplay.setCursor(col1, row);
  BoxDisplay::baseDisplay.print(BoxSensMeasure::values.gasNameA);
  BoxDisplay::baseDisplay.setCursor(col2, row);
  BoxDisplay::baseDisplay.print(formatString(String(BoxSensMeasure::values.probabilityA, 2), FORMAT_CELL_PERCENT)); 
  row = row + 8;

  BoxDisplay::baseDisplay.setCursor(col1, row);
  BoxDisplay::baseDisplay.print(BoxSensMeasure::values.gasNameB);
  BoxDisplay::baseDisplay.setCursor(col2, row);
  BoxDisplay::baseDisplay.print(formatString(String(BoxSensMeasure::values.probabilityB, 2), FORMAT_CELL_PERCENT)); 
  row = row + 8;

  if (BoxSensMeasure::values.gasNameC != "") {
    BoxDisplay::baseDisplay.setCursor(col1, row);
    BoxDisplay::baseDisplay.print(BoxSensMeasure::values.gasNameC);
    BoxDisplay::baseDisplay.setCursor(col2, row);
    BoxDisplay::baseDisplay.print(formatString(String(BoxSensMeasure::values.probabilityC, 2), FORMAT_CELL_PERCENT)); 
    row = row + 8;
  }

  if (BoxSensMeasure::values.gasNameD != "") {
    BoxDisplay::baseDisplay.setCursor(col1, row);
    BoxDisplay::baseDisplay.print(BoxSensMeasure::values.gasNameD);
    BoxDisplay::baseDisplay.setCursor(col2, row);
    BoxDisplay::baseDisplay.print(formatString(String(BoxSensMeasure::values.probabilityD, 2), FORMAT_CELL_PERCENT)); 
  }

  BoxDisplay::printTime();
  // BoxDisplay::baseDisplay.setCursor(103, 24);
  // BoxDisplay::baseDisplay.print(formatString(String(BoxSensMeasure::values.accuracy), "%4s"));

  BoxDisplay::baseDisplay.display();

}


