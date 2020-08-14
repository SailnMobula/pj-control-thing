#include "battery_info.h"
#include "led_service.h"
#include "samd.h"
#include "timed_service.h"

#include <ArduinoBLE.h>

#define LED_OUTPUT_1 2

int ledState = LOW;
int previousBatteryLevel = 0;
int previousBatteryVoltage = 0;
int previousLedDutyCycle = 0;
uint32_t ledServiceState = 1;

led_service ledService(200, 2, 1000);
timed_service longTimedService((uint32_t)5000 * 1000);
timed_service shortTimedService((uint32_t)1 * 1000);

BLEService bleBatteryService("9f832063-bbe1-45b2-b9cb-b4c7b2a0c36a");
BLEUnsignedCharCharacteristic
    batteryLevelCharacteristic("cd3d8382-99b8-434e-b221-74d18e68df9a",
                               BLERead | BLENotify);

BLEUnsignedCharCharacteristic
    batteryVoltageCharacteristic("d5e05399-8d79-40bd-afaf-db7b63eb6035",
                                 BLERead | BLENotify);

BLEService bleLedService("98194a8e-a697-4b49-93f5-25ca2602013c");
BLEUnsignedCharCharacteristic
    leftLightDutyCycleCharacteristic("4d5c48e8-509c-4f61-a39d-ccb8f78dee34",
                                     BLEWrite | BLENotify);
// BLEUnsignedCharCharacteristic
//     rightLightDutyCycleCharacteristic("901960d4-1f3b-4898-a6f9-afe0f9044114",
//                                       BLEWrite | BLENotify);

void bleInit() {
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1)
      ;
  }

  BLE.setDeviceName("PJMonitor");
  BLE.setLocalName("BatteryMonitor");

  BLE.setAdvertisedService(bleBatteryService);
  bleBatteryService.addCharacteristic(batteryLevelCharacteristic);
  bleBatteryService.addCharacteristic(batteryVoltageCharacteristic);
  BLE.addService(bleBatteryService);

  BLE.setAdvertisedService(bleLedService);
  bleLedService.addCharacteristic(leftLightDutyCycleCharacteristic);
  // bleLedService.addCharacteristic(rightLightDutyCycleCharacteristic);
  BLE.addService(bleLedService);

  BLE.advertise();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Serial initalized");

  pinMode(LED_BUILTIN, OUTPUT);

  bleInit();
}

void blinkLED() {
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LED_BUILTIN, ledState);
}

void bleSendDutyCycle(BLEDevice central, uint8_t currentLedDutyCycle) {
  if (!central) {
    return;
  }
  if (!central.connected()) {
    return;
  }

  if (leftLightDutyCycleCharacteristic.written()) {
    uint8_t ledDimmValue;
    ledService.turnOnByDutyCycle(leftLightDutyCycleCharacteristic.value());
  }
}

void bleSendBattery(BLEDevice central, uint8_t batteryVoltage) {
  if (!central) {
    return;
  }
  if (!central.connected()) {
    return;
  }

  uint8_t batteryLevel = 82;
  if (batteryLevel != previousBatteryLevel) {
    previousBatteryLevel = batteryLevel;
    batteryLevelCharacteristic.writeValue(batteryLevel);
  }

  if (batteryVoltage != previousBatteryVoltage) {
    previousBatteryVoltage = batteryVoltage;
    batteryVoltageCharacteristic.writeValue(batteryVoltage);
  }
}

void loop() {
  int batteryLevel;
  uint8_t batteryVoltage;
  uint8_t currentDutyCycle;
  uint32_t batteryVoltageRaw;
  // ADC_LSB =  3300 * 1000 / 4095;
  // double_t VOLTAGE_DIVIDER =  12200 / 2200;

  // analogReadResolution(12);

  ledService.controlLed();

  BLEDevice central = BLE.central();

  if (shortTimedService.isTimeSlotActive()) {
    currentDutyCycle = ledService.getCurrentDutyCycle();
    bleSendDutyCycle(central, currentDutyCycle);
  }

  if (longTimedService.isTimeSlotActive()) {
    blinkLED();
    batteryVoltageRaw = analogRead(A0);
    batteryVoltage = ((((batteryVoltageRaw)*12200) / 1023) * 3300) / 2200 / 100;
    bleSendBattery(central, batteryVoltage);
    Serial.println(batteryVoltage);
  }
}
