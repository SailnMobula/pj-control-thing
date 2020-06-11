#ifndef LIN_DRIVER_H
#define LIN_DRIVER_H

#include <Arduino.h>

class lin_driver {

public:
  lin_driver();
  lin_driver(uint32_t baudrate);

  void sendMessageRequest(uint8_t messageId);
  int writeMessge(uint8_t messageId, uint8_t *data, size_t data_size);

private:
  uint32_t const defaultBaudrate = 19200;
  bool trySendSerial(const uint8_t *data, size_t length);
  bool trySendSerial(const uint8_t data);
  void wakeBus();
  void sendSyncBreak();
  void sendSync();
  void sendPID(uint8_t messageId);
  uint8_t calcPId(uint8_t messageId);
  uint8_t calcIdentParity(uint8_t ident);
  uint8_t calcClassicChecksum(uint8_t *data, size_t data_size);
  uint8_t calcEnhancedChecksum(uint8_t pID, uint8_t *data, size_t data_size);
  void printLinRxDataBufferChar(uint8_t channel, uint8_t *data,
                                size_t dataLength);
  int8_t tryReadMessageResponse(uint8_t *rxBuffer, size_t length);
};

#endif
