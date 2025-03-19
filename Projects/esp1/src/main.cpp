#include <Arduino.h>
#include <Lin_Interface.hpp>

// Define UART pins for ESP32 (UART2)
#define LIN_RX_PIN 16      // RX pin
#define LIN_TX_PIN 17      // TX pin
#define LIN_BAUDRATE 19200 // Common LIN baud rate

// Create LIN interface object using UART2
Lin_Interface linBus(2);

void setup()
{
  // Start Serial for debugging
  Serial.begin(115200);
  delay(1000);

  // Initialize the LIN bus
  linBus.begin(LIN_BAUDRATE, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);

  Serial.println("LIN Library Test: Bus Initialized");
}

void loop()
{
  // Test 1: Send a LIN wakeup command
  linBus.writeCmdWakeup();
  Serial.println("Test 1: Sent LIN Wakeup Command");

  delay(2000); // Wait 2 seconds

  // Test 2: Send a simple LIN frame
  uint8_t testData[] = {0xAA, 0xBB};    // Dummy data
  linBus.writeFrame(0x10, testData, 2); // PID 0x10, 2 bytes
  Serial.println("Test 2: Sent LIN Frame (PID 0x10, Data: 0xAA 0xBB)");

  delay(2000); // Wait 2 seconds
}