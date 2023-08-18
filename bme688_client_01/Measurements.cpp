#include "Measurements.h"

int Measurements::memBufferSize = 1028;
int Measurements::memBufferIndx = 0;
ValuesColl* Measurements::measurements;

void Measurements::begin() {

  // https://www.esp32.com/viewtopic.php?t=27771
  Measurements::measurements = (ValuesColl*) ps_malloc(Measurements::memBufferSize * sizeof(ValuesColl));
  memset(Measurements::measurements, 0, Measurements::memBufferSize * sizeof(ValuesColl));

}

ValuesColl Measurements::getOffsetMeasurement(int offset) {
  return Measurements::measurements[(Measurements::memBufferIndx - offset - 1) % Measurements::memBufferSize];
}

ValuesColl Measurements::getLatestMeasurement() {
  return Measurements::measurements[(Measurements::memBufferIndx - 1) % Measurements::memBufferSize];
}

void Measurements::putMeasurement(ValuesColl measurement) {
  Measurements::measurements[Measurements::memBufferIndx % Measurements::memBufferSize] = measurement;
  memBufferIndx++;
}

ValuesColl Measurements::getMeasurement(int memIndex) {
  return Measurements::measurements[memIndex];
}

int Measurements::getFirstPublishableIndex() {  
  // oldest first, this iterates the full array (if there is a single measurement only)
  for (int index = memBufferIndx; index < Measurements::memBufferSize + memBufferIndx; index++) {
    if (Measurements::measurements[index % memBufferIndx].publishable) {
      return index % memBufferIndx;
    }
  }
  return -1;
}

int Measurements::getPublishableCount() {  
  int publishableCount = 0;
  for (int index = 0; index < Measurements::memBufferSize; index++) {
    if (Measurements::measurements[index].publishable) {
      publishableCount++;
    }
  }
  return publishableCount;
}

void Measurements::setPublished(int memIndex) {
  Measurements::measurements[memIndex].publishable = false;
}
