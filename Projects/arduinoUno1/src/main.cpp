#include <Arduino.h>

#define LED_Butt 13 // Define the LED pin (Using built-in LED on pin 13)

void setup()
{
  pinMode(LED_Butt, OUTPUT);
}

void loop()
{
  digitalWrite(LED_Butt, HIGH);
  delay(200);
  digitalWrite(LED_Butt, LOW);
  delay(200);
}
