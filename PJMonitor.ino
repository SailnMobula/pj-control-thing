#include "battery_info.h"
#include "ibs_service.h"

int ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;
int previousBatteryLevel = 0;

ibs_service ibsService;

void setup()
{
    Serial.begin(9600);
    // while (!Serial);
    Serial.println("Serial initalized");

    pinMode(LED_BUILTIN, OUTPUT);

    ibs_service ibsService;
}

void serializeData(char *buffer, int temperature, int humidity, float x, float y, float z)
{
    sprintf(buffer, "{\"data\":{\"temperature\":%d,\"humidity\":%d,\"accelerometerX\":%.2f,\"accelerometerY\":%.2f,\"accelerometerZ\":%.2f}}\r\n", temperature, humidity, x, y, z);
}

bool isTimeSlotActive()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        return true;
    }

    return false;
}

void blinkLED()
{
    if (ledState == LOW)
    {
        ledState = HIGH;
    }
    else
    {
        ledState = LOW;
    }

    digitalWrite(LED_BUILTIN, ledState);
}

void loop()
{
    // put your main code here, to run repeatedly:
    char serializedString[256];
    float accelerationValues[3] = {0.0, 0.0, 0.0};
    int temperature;
    int humidity;
    int batteryLevel;

    if (isTimeSlotActive())
    {
        blinkLED();
        ibsService.read();
        delay(10);
        ibsService.write();
    }
}
