#include "MyLIN.h"

MyLIN::MyLIN(uint8_t uartNum, int8_t txPin)
{
    if (uartNum == 1)
        serial = &Serial1;
    else if (uartNum == 2)
        serial = &Serial2;
    else
        serial = &Serial;
    this->txPin = txPin; // Store the TX pin
}

void MyLIN::begin(unsigned long baud)
{
    if (txPin != -1)
    {
        serial->begin(baud, SERIAL_8N1, -1, txPin); // Use custom TX pin if specified
    }
    else
    {
        serial->begin(baud); // Use default pins otherwise
    }
}

void MyLIN::sendBreak()
{
    serial->end(); // Stop UART
    if (txPin == -1)
        txPin = 17; // Default to GPIO17 for UART2 if not specified
    pinMode(txPin, OUTPUT);
    digitalWrite(txPin, LOW);  // Pull TX low for break
    delayMicroseconds(1000);   // ~19 bits at 19200 baud
    digitalWrite(txPin, HIGH); // Release TX
    if (txPin != -1)
    {
        serial->begin(19200, SERIAL_8N1, -1, txPin); // Restart with custom TX
    }
    else
    {
        serial->begin(19200); // Restart with default pins
    }
}

uint8_t MyLIN::calculatePID(uint8_t frameID)
{
    frameID &= 0x3F;
    uint8_t p0 = (bitRead(frameID, 0) ^ bitRead(frameID, 1) ^ bitRead(frameID, 2) ^ bitRead(frameID, 4));
    uint8_t p1 = !(bitRead(frameID, 1) ^ bitRead(frameID, 3) ^ bitRead(frameID, 4) ^ bitRead(frameID, 5));
    return (frameID | (p0 << 6) | (p1 << 7));
}

uint8_t MyLIN::calculateChecksum(uint8_t pid, uint8_t *data, uint8_t len)
{
    uint16_t sum = pid;
    for (uint8_t i = 0; i < len; i++)
    {
        sum += data[i];
        if (sum >= 256)
            sum -= 255;
    }
    return ~sum;
}

void MyLIN::sendFrame(uint8_t frameID, uint8_t *data, uint8_t dataLen)
{
    uint8_t pid = calculatePID(frameID);
    sendBreak();
    serial->write(0x55);
    serial->write(pid);
    for (uint8_t i = 0; i < dataLen; i++)
    {
        serial->write(data[i]);
    }
    uint8_t checksum = calculateChecksum(pid, data, dataLen);
    serial->write(checksum);
}

bool MyLIN::readFrame(uint8_t frameID, uint8_t *buffer, uint8_t expectedLen)
{
    uint8_t pid = calculatePID(frameID);
    sendBreak();
    serial->write(0x55);
    serial->write(pid);
    unsigned long timeout = millis() + 100;
    while (serial->available() < (expectedLen + 1) && millis() < timeout)
    {
        delay(1);
    }
    if (serial->available() >= (expectedLen + 1))
    {
        for (uint8_t i = 0; i < expectedLen; i++)
        {
            buffer[i] = serial->read();
        }
        uint8_t receivedChecksum = serial->read();
        uint8_t calculatedChecksum = calculateChecksum(pid, buffer, expectedLen);
        return (receivedChecksum == calculatedChecksum);
    }
    return false;
}