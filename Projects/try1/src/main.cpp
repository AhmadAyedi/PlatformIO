#include <Arduino.h>
#include "MyLIN.h"

#define LIN_UART_NR 2 // Use UART2 (TX=GPIO17, RX=GPIO16)
#define LIN_BAUDRATE 19200

MyLIN lin(LIN_UART_NR);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 LIN Master Starting...");
  lin.begin(LIN_BAUDRATE);
}

void loop()
{
  // Test 1: Send a frame with data
  uint8_t data[] = {0x12, 0x34};
  lin.sendFrame(0x01, data, 2);
  Serial.println("Sent frame ID 0x01 with data 0x12 0x34");

  // Test 2: Request data from slave (ID 0x02)
  uint8_t buffer[2] = {0};
  if (lin.readFrame(0x02, buffer, 2))
  {
    Serial.print("Received from slave: ");
    Serial.print(buffer[0], HEX);
    Serial.print(" ");
    Serial.println(buffer[1], HEX);
  }
  else
  {
    Serial.println("No response from slave");
  }

  delay(2000);
}