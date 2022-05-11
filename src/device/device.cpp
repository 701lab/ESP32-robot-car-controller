#include "device.h"

void esp32_setup_peripherals(void)
{
  Serial.begin(115200);
  Serial2.begin(115200);
//   pinMode(16, OUTPUT);
//   digitalWrite(16, HIGH); //?

  Serial.println();
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}