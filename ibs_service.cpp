#include "ibs_service.h"

int8_t ibs_service::read()
{
    ibsLin.sendMessageRequest(0x30);
    return 1;
}

int8_t ibs_service::bruteforceRead()
{
    ibsLin.sendMessageRequest(currentMessageIdToSend);

    if (currentMessageIdToSend >= MAX_MESSAGE_ID)
    {
        currentMessageIdToSend = 0;
    }
    else
    {
        currentMessageIdToSend++;
    }

    return 1;
}

int8_t ibs_service::write()
{
    uint8_t messageBody[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    ibsLin.writeMessge(0x35, messageBody, sizeof(messageBody));
    return 1;
}
