#include "lin_driver.h"

lin_driver::lin_driver() {
    lin_driver(defaultBaudrate);
}

lin_driver::lin_driver(uint32_t baudrate) {
    Serial1.begin(baudrate);
}

void lin_driver::sendMessageRequest(uint8_t messageId)
{
    uint8_t rxBuffer[8];

    Serial.print("Send Message Request 0x");
    Serial.println(messageId, HEX);
    wakeBus();
    sendSyncBreak();
    sendSync();
    sendPID(messageId);
    delay(5);
    tryReadMessageResponse(rxBuffer, 8);
}

int lin_driver::writeMessge(uint8_t messageId, uint8_t *data, size_t data_size)
{
    uint8_t rxBuffer[8];
    uint8_t checksum = calcEnhancedChecksum(calcPId(messageId), data, data_size);

    sendSyncBreak();
    sendSync();
    sendPID(messageId);

    trySendSerial(data, data_size);
    trySendSerial(checksum);

    delay(5);
    tryReadMessageResponse(rxBuffer, sizeof(rxBuffer));
    return 1;
}

bool lin_driver::trySendSerial(const uint8_t *data, size_t length)
{
    int sentBytes = Serial1.write(data, length);
    return true;
}

bool lin_driver::trySendSerial(const uint8_t data)
{
    int sentBytes = Serial1.write(data);
    return true;
}

void lin_driver::wakeBus()
{
    uint8_t wakeupPattern[2] = {0x80, 0x00};
    trySendSerial(wakeupPattern, 2);
}

void lin_driver::sendSyncBreak()
{
    uint8_t syncBreakPattern[2] = {0xC0, 0x01};
    trySendSerial(syncBreakPattern, 2);
}

void lin_driver::sendSync()
{
    uint8_t syncPattern[1] = {0x55};
    trySendSerial(syncPattern, sizeof(syncPattern));
}

void lin_driver::sendPID(uint8_t messageId)
{
    uint8_t identByte = calcPId(messageId);
    uint8_t pid[1] = {identByte};
    trySendSerial(pid, sizeof(pid));
}

uint8_t lin_driver::calcPId(uint8_t messageId)
{
    return ((messageId & 0x3f) | calcIdentParity(messageId));
}

uint8_t lin_driver::calcIdentParity(uint8_t ident)
{
    uint8_t p0 = bitRead(ident, 0) ^ bitRead(ident, 1) ^ bitRead(ident, 2) ^ bitRead(ident, 4);
    uint8_t p1 = ~(bitRead(ident, 1) ^ bitRead(ident, 3) ^ bitRead(ident, 4) ^ bitRead(ident, 5));
    return (p0 | (p1 << 1)) << 6;
}

uint8_t lin_driver::calcClassicChecksum(uint8_t data[], size_t data_size)
{
    uint8_t suma = 0;
    for (int i = 2; i < data_size - 1; i++)
        suma = suma + data[i];
    uint8_t v_checksum = 255 - suma - 1;

    return v_checksum;
}

uint8_t lin_driver::calcEnhancedChecksum(uint8_t pID, uint8_t data[], size_t data_size)
{
    uint8_t receivedChecksum = data[data_size - 1];
    uint32_t calc = pID;

    for (int i = 0; i < data_size; i++)
    {
        calc += data[i];
        if (calc >= 256)
            calc -= 255;
    }
    uint8_t calculatedChecksum = 255 - calc;

    return calculatedChecksum;
}

void lin_driver::printLinRxDataBufferChar(uint8_t channel, uint8_t *data, size_t dataLength)
{
    int i = 0;
    Serial.print("### Channel:");
    Serial.print(channel, DEC);
    Serial.print(" ### ID  ");
    for (i = 0; i < dataLength; i++)
    {
        Serial.print(" ## 0x");
        Serial.print(data[i], HEX);
    }
    Serial.print("\n");
}

int8_t lin_driver::tryReadMessageResponse(uint8_t *rxBuffer, size_t length)
{

    int success = -1;
    uint8_t rec[256];
    uint8_t rxBytesLengthToRead = 0;
    rxBytesLengthToRead = Serial1.available();

    if (rxBytesLengthToRead)
    {
        Serial1.readBytes(rec, rxBytesLengthToRead);
        printLinRxDataBufferChar(1, rec, rxBytesLengthToRead);
        success = 1;
    }
    else
    {
        success = -1; // no data available on LIN bus
        Serial.println("No data on LIN bus");
    }

    return success;
}