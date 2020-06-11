#ifndef BATTERY_INFO_H
#define BATTERY_INFO_H

struct batteryInfo {
  uint8_t isBatteryInfoValid = 0;
  int8_t cntRecvInvalidMsgs = 0;
  uint8_t SoC;
  uint8_t SoH;
  uint8_t batteryTemp;
  uint8_t availCap;
  float U_Voltage;
  float I_Current;
};

#endif
