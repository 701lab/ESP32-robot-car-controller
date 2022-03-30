#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
}

void loop() {
  digitalWrite(16, !digitalRead(16));
  delay(600);
  // put your main code here, to run repeatedly:
}