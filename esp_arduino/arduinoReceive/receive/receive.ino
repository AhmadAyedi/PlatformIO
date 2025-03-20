void setup() {
  Serial.begin(19200, SERIAL_8N1); // LIN baud rate: 19.2k, 8 data bits, no parity, 1 stop bit
  while (!Serial) {
    ; // Wait for Serial to initialize
  }
  Serial.println("LIN Slave Test Started");
}

void loop() {
  // Wait for the Sync byte (0x55) to indicate the start of a frame
  if (Serial.available() > 0 && Serial.read() == 0x55) {
    // Sync byte detected, now read the PID
    while (Serial.available() == 0) {
      // Wait for PID byte
    }
    uint8_t pid = Serial.read();

    // Assuming Frame ID is 0x10 (your ESP32 setting), calculate expected PID
    uint8_t expectedPid = calculatePid(0x10);
    if (pid == expectedPid) {
      // PID matches, read 3 data bytes
      uint8_t data[3];
      for (int i = 0; i < 3; i++) {
        while (Serial.available() == 0) {
          // Wait for each data byte
        }
        data[i] = Serial.read();
      }

      // Read the checksum
      while (Serial.available() == 0) {
        // Wait for checksum byte
      }
      uint8_t receivedChecksum = Serial.read();

      // Calculate expected checksum (LIN 2.x: over PID + Data)
      uint8_t calculatedChecksum = calculateChecksum(pid, data, 3);

      // Verify checksum
      if (receivedChecksum == calculatedChecksum) {
        Serial.print("Received frame with ID 0x10, Data: ");
        for (int i = 0; i < 3; i++) {
          Serial.print("0x");
          if (data[i] < 0x10) Serial.print("0"); // Add leading zero for single-digit hex
          Serial.print(data[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      } else {
        Serial.println("Checksum mismatch");
      }
    } else {
      Serial.println("PID mismatch");
    }
  }
}

// Function to calculate Protected ID (PID) from Frame ID
uint8_t calculatePid(uint8_t frameId) {
  // LIN 2.x PID: Frame ID (6 bits) + 2 parity bits
  uint8_t p0 = (bitRead(frameId, 0) ^ bitRead(frameId, 1) ^ bitRead(frameId, 2) ^ bitRead(frameId, 4));
  uint8_t p1 = !(bitRead(frameId, 1) ^ bitRead(frameId, 3) ^ bitRead(frameId, 4) ^ bitRead(frameId, 5));
  return (frameId & 0x3F) | (p0 << 6) | (p1 << 7);
}

// Function to calculate LIN 2.x checksum (enhanced: over PID + Data)
uint8_t calculateChecksum(uint8_t pid, uint8_t* data, uint8_t dataLen) {
  uint16_t sum = pid; // Start with PID for LIN 2.x
  for (uint8_t i = 0; i < dataLen; i++) {
    sum += data[i];
    if (sum >= 256) {
      sum -= 255; // Carry wrap-around
    }
  }
  return ~sum; // Invert the result
}