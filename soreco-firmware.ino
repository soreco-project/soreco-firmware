/************************************************************************************************** 
 Firmware of soreco Sonos Remote Control based on Arduino and ESP8266.
 This is the entry point for Arduino applications (also known as sketch).
 *************************************************************************************************/

#include <Arduino.h>
#include "src/DebugConsole/DebugConsole.h"
#include "src/DeviceSettings/DeviceSettings.h"
#include "src/WifiManager/WifiManager.h"
#include "src/Sonos/SonosDevice.h"
#include "src/DeviceState/DeviceStateMachine.h"

const char* FIRMWARE_VERSION = "0.0.1";

DebugConsole debugConsole;
WifiManager wifiManager;
SonosDevice sonosDevice;
DeviceStateMachine deviceStateMachine(wifiManager, sonosDevice);

void setup() {
    // // Note: try to use flash strings to reduce RAM usage!
    // https://espressif.com/sites/default/files/documentation/save_esp8266ex_ram_with_progmem_en.pdf   
    Serial.begin(115200); Serial.println();
    Serial.print(F("Firmware soreco V")); Serial.println(FIRMWARE_VERSION);
    Serial.print(F("Compiletime: ")); Serial.print(__TIME__); Serial.print(" "); Serial.println(__DATE__);
    Serial.print(F("ESP8266 SDK: ")); Serial.println(ESP.getSdkVersion());

    DeviceSettings::load();
    debugConsole.setup(wifiManager, sonosDevice);
    wifiManager.setup();
    deviceStateMachine.resetStateMachine();
}

void loop() {
    debugConsole.loop();
    wifiManager.loop(); // TODO move to state machine
    deviceStateMachine.runStateMachine();
}
