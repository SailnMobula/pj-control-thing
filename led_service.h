#ifndef LED_SERVICE_H
#define LED_SERVICE_H

#include "timed_service.h"
#include <Arduino.h>

class led_service : public timed_service {
private:
  static const uint32_t DUTY_CYCLE_MAX = 100;
  static const uint32_t DUTY_CYCLE_MIN = 0;
  uint32_t ledOutputPin;

  float currentDutyCycle = 0;
  uint32_t targetDutyCycle = 0;
  uint32_t dutyCycleCounter = 0;

  boolean shouldFade = false;
  float fadeConstant = 0;

  uint32_t static calcDutyCycleLsbInterval(float frequency);
  float determineCurrentDutyCycle();
  float getCalculatedFadedDutyCycle();
  uint32_t limitDutyCycle(uint32_t dutyCycle);
  void setLedState(uint32_t state);

public:
  led_service(float frequency, uint8_t outputPin, uint32_t fadeTimeMillis);
  ~led_service();

  void turnOn();
  void turnOff();
  void turnOnByDutyCycle(uint32_t dutyCycle);
  void controlLed();
};

#endif
