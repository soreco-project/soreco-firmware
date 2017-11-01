/*************************************************** 
 Firmware of soreco Sonos Remote Control.
 Note: based on Arduino and ESP8266
 ****************************************************/

#include "Arduino.h"

const char FIRMWARE_VERSION[]="0.0.1";

void setup() {
  Serial.begin(115200);
  Serial.print("SoReMo FW "); Serial.println(FIRMWARE_VERSION);
  Serial.print("Compiletime: "); Serial.print(__TIME__); Serial.print(" "); Serial.println(__DATE__);
}

void loop() {
}
