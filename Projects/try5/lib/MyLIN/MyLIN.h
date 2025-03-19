#ifndef MYLIN_H
#define MYLIN_H

#include <Arduino.h>

class MyLIN
{
public:
    MyLIN(uint8_t uartNum, int8_t txPin = -1); // Add txPin parameter, default -1
    void begin(unsigned long baud);
    void sendBreak();
    void sendFrame(uint8_t frameID, uint8_t *data, uint8_t dataLen);
    bool readFrame(uint8_t frameID, uint8_t *buffer, uint8_t expectedLen);

private:
    HardwareSerial *serial;
    int8_t txPin; // Store TX pin number
    uint8_t calculatePID(uint8_t frameID);
    uint8_t calculateChecksum(uint8_t pid, uint8_t *data, uint8_t len);
};

#endif