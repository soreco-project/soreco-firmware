#include "Arduino.h"

const char version[]="0.0.1";
const char compiletime[]=__TIME__;
const char compiledate[]=__DATE__;

void setup() {
  Serial.begin(115200);
  Serial.print("SoReMo FW "); Serial.println(version);
  Serial.print("compiletime: "); Serial.print(compiletime); Serial.print(" "); Serial.println(compiledate);
}

void loop() {
  Serial.println("loop");
  delay(1000);
}
