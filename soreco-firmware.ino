/*************************************************** 
 Firmware of soreco Sonos Remote Control.
 Note: based on Arduino and ESP8266
 ****************************************************/

#include "Arduino.h"
#include "DebugConsole.h"
#include "DeviceSettings.h"
#include "WifiManager.h"

const char* FIRMWARE_VERSION = "0.0.1";

DebugConsole debugConsole;
WifiManager wifiManager;

void setup() {
    Serial.begin(115200); Serial.println();
    Serial.print("Firmware soreco V"); Serial.println(FIRMWARE_VERSION);
    Serial.print("Compiletime: "); Serial.print(__TIME__); Serial.print(" "); Serial.println(__DATE__);
    Serial.print("ESP8266 SDK: "); Serial.println(ESP.getSdkVersion());

    DeviceSettings::load();
    debugConsole.setup();
    wifiManager.setup();
    wifiManager.startServiceMode();
}

void loop() {
    debugConsole.loop();
    wifiManager.loop();
}
