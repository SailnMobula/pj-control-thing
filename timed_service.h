#ifndef TIMED_SERVICE_H
#define TIMED_SERVICE_H

#include <Arduino.h>

class timed_service {
private:
  uint32_t cycleTimeMicroseconds = 0;
  uint32_t previousMicros = 0;

public:
  timed_service(uint32_t frequency);
  ~timed_service();

  boolean isTimeSlotActive();
  uint32_t getCurrentInterval();
  uint32_t getCallsPerSecond();
  uint32_t getCallsPerMillis();
};

#endif