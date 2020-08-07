#include "battery_info.h"
// #include "ibs_service.h"
#include "led_service.h"
#include "samd.h"

#define LED_OUTPUT_1 2

int ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 5000;
int previousBatteryLevel = 0;

uint32_t ledServiceState = 1;

// ibs_service ibsService;
led_service ledService(200, 2, 2*1000);

void setup() {
  Serial.begin(9600);
  // while (!Serial);
  Serial.println("Serial initalized");
  // Serial1.begin(19200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_OUTPUT_1, OUTPUT);
  // pinMode(D2, OUTPUT);

  // ibs_service ibsService;
}

void serializeData(char *buffer, int temperature, int humidity, float x,
                   float y, float z) {
  sprintf(buffer,
          "{\"data\":{\"temperature\":%d,\"humidity\":%d,\"accelerometerX\":%."
          "2f,\"accelerometerY\":%.2f,\"accelerometerZ\":%.2f}}\r\n",
          temperature, humidity, x, y, z);
}

bool isTimeSlotActive() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    return true;
  }

  return false;
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

// void controlLed(uint32_t dutyCycle, uint32_t frequency) {

//   uint32_t cycleTimeMicroseconds = 10000;
//   uint32_t tmpDutyCycle = dutyCycle;
//   if (tmpDutyCycle > 100) {
//     tmpDutyCycle = 100;
//   }

//   if (tmpDutyCycle < 0) {
//     tmpDutyCycle = 0;
//   }

//   uint32_t highTime = cycleTimeMicroseconds * dutyCycle / 100;
//   uint32_t lowTime = cycleTimeMicroseconds - highTime;

//   if (highTime < 10) {
//     digitalWrite(LED_OUTPUT_1, LOW);
//     return;
//   }

//   digitalWrite(LED_OUTPUT_1, HIGH);
//   delayMicroseconds(highTime);
//   digitalWrite(LED_OUTPUT_1, LOW);
//   delayMicroseconds(lowTime);
// }

void loop() {
  // put your main code here, to run repeatedly:
  char serializedString[256];
  float accelerationValues[3] = {0.0, 0.0, 0.0};
  int temperature;
  int humidity;
  int batteryLevel;

  ledService.controlLed();

  if (isTimeSlotActive()) {
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
