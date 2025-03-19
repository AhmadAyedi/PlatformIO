#include <Arduino.h>
#include "Lin-Interface/src/Lin_Interface.hpp" // Adjusted path

#define LIN_UART_NR 2
#define LIN_BAUDRATE 19200

Lin_Interface linBus(LIN_UART_NR);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting LIN Interface Test...");

  linBus.begin(LIN_BAUDRATE);
  Serial.println("LIN bus initialized at 19200 baud");

  linBus.writeCmdWakeup();
  Serial.println("Sent LIN wakeup command");
}

void loop()
{
  bool success = linBus.writeDiagnosticMasterRequest();
  if (success)
  {
    Serial.println("Diagnostic Master Request sent successfully");
  }
  else
  {
    Serial.println("Failed to send Diagnostic Master Request");
  }

  linBus.LinMessage.fill(0);
  if (linBus.requestFrame(0x3D))
  {
    Serial.print("Received frame data: ");
    for (int i = 0; i < 8; i++)
    {
      Serial.print(linBus.LinMessage[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Checksum: ");
    Serial.println(linBus.LinMessage[8], HEX);
  }
  else
  {
    Serial.println("No response or invalid frame received");
  }

  delay(2000);
}