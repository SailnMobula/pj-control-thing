#include "battery_info.h"
// #include "ibs_service.h"
#include "led_service.h"
#include "samd.h"
#include "timed_service.h"

#define LED_OUTPUT_1 2

int ledState = LOW;
int previousBatteryLevel = 0;

uint32_t ledServiceState = 1;

// ibs_service ibsService;
led_service ledService(200, 2, 2000);
timed_service timedService((uint32_t)5000 * 1000);

void setup() {
  Serial.begin(9600);
  // while (!Serial);
  Serial.println("Serial initalized");

  pinMode(LED_BUILTIN, OUTPUT);
}

void serializeData(char *buffer, int temperature, int humidity, float x,
                   float y, float z) {
  sprintf(buffer,
          "{\"data\":{\"temperature\":%d,\"humidity\":%d,\"accelerometerX\":%."
          "2f,\"accelerometerY\":%.2f,\"accelerometerZ\":%.2f}}\r\n",
          temperature, humidity, x, y, z);
}

void blinkLED() {
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }

  digitalWrite(LED_BUILTIN, ledState);
}

void switchLedState() {

  switch (ledServiceState) {
  case 1:
    ledService.turnOn();
    ledServiceState = 2;
    break;
  case 2:
    ledService.turnOnByDutyCycle(10);
    ledServiceState = 3;
    break;
  case 3:
    ledService.turnOnByDutyCycle(70);
    ledServiceState = 4;
    break;
  case 4:
    ledService.turnOff();
    ledServiceState = 1;
    break;

  default:
    ledServiceState = 1;
    break;
  }
}

void loop() {
  char serializedString[256];
  float accelerationValues[3] = {0.0, 0.0, 0.0};
  int temperature;
  int humidity;
  int batteryLevel;

  ledService.controlLed();

  if (timedService.isTimeSlotActive()) {
    blinkLED();
    Serial.println("Heartbeat");
    switchLedState();
    // ibsService.read();
    // ibsService.write();
    // ibsService.bruteforceRead();
    // delay(10);
    // ibsService.write();
  }
}
