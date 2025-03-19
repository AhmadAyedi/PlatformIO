#include <Arduino.h>

#define LED_Butt 5 // Built-in LED on Wemos LOLIN32 (Change if using an external LED)

void setup()
{
  pinMode(LED_Butt, OUTPUT);
}

void loop()
{
  digitalWrite(LED_Butt, HIGH);
  delay(1000);
  digitalWrite(LED_Butt, LOW);
  delay(1000);
}
