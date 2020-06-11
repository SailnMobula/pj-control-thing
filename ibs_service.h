#ifndef IBS_SERVICE_H
#define IBS_SERVICE_H

#include "lin_driver.h"
#include <Arduino.h>

class ibs_service
{
public:
    int8_t read();
    int8_t bruteforceRead();
    int8_t write();

private:
    lin_driver ibsLin;
    uint8_t currentMessageIdToSend = 0x00;
    const uint8_t MAX_MESSAGE_ID = 63;
};

#endif