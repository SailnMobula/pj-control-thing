#include "timed_service.h"
#include <Arduino.h>

timed_service::timed_service(uint32_t intervalInMicros) {
  this->intervalInMicros = intervalInMicros;
}

timed_service::~timed_service() {}

bool timed_service::isTimeSlotActive() {
  unsigned long currentMicros = micros();

  if (getCurrentInterval() > intervalInMicros) {
    previousMicros = currentMicros;
    return true;
  }

  return false;
}

uint32_t timed_service::getCurrentInterval() {
  unsigned long currentMicros = micros();
  return currentMicros - previousMicros;
}

uint32_t timed_service::getCallsPerMillis() { return 1000 / intervalInMicros; }

uint32_t timed_service::getCallsPerSecond() {
  return 1000 * 1000 / intervalInMicros;
}