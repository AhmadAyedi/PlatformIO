#include <Arduino.h>

#define LIN_BAUDRATE 19200

// Function to calculate LIN enhanced checksum (PID + data)
uint8_t calculateChecksum(uint8_t pid, uint8_t *data, uint8_t len)
{
  uint16_t sum = pid; // Include PID for enhanced checksum
  for (uint8_t i = 0; i < len; i++)
  {
    sum += data[i];
    if (sum >= 256)
      sum -= 255; // Carry wrap-around
  }
  return ~sum; // Invert result
}

void setup()
{
  Serial.begin(LIN_BAUDRATE);   // Start UART at LIN baud rate
  pinMode(LED_BUILTIN, OUTPUT); // Use built-in LED for feedback
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  // Wait for at least 3 bytes: Break (detected indirectly), Sync, PID
  if (Serial.available() >= 2)
  { // Sync + PID minimum
    // LIN break isn’t directly detectable via UART, so we rely on Sync
    uint8_t sync = Serial.read();
    if (sync == 0x55)
    { // Check for LIN sync byte
      uint8_t pid = Serial.read();
      if (pid == 0x82)
      { // PID for frame ID 0x02 (calculated by master)
        // Slave response: send 2 bytes of data + checksum
        uint8_t data[] = {0xAB, 0xCD};
        uint8_t checksum = calculateChecksum(pid, data, 2);

        Serial.write(data, 2);  // Send data
        Serial.write(checksum); // Send checksum

        // Blink LED to indicate response
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    // Clear any remaining bytes in buffer
    while (Serial.available())
      Serial.read();
  }
}