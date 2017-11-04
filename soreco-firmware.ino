/************************************************************************************************** 
 Firmware of soreco Sonos Remote Control based on Arduino and ESP8266.
 This is the entry point for Arduino applications (also known as sketch).
 *************************************************************************************************/

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
    debugConsole.setup(wifiManager);
    wifiManager.setup();

    DeviceSettings::WiFiConfig wifiConfig = DeviceSettings::getWiFiConfig();
    if (wifiConfig.isConfigured()) {
        Serial.println("Connecting to configured WiFi");
        wifiManager.startClientMode(wifiConfig.ssid, wifiConfig.passphrase);
    }
    else {
        // only enter config mode when there is no stored network in DeviceSettings
        Serial.println("Starting WiFi hotspot for configuration");
        wifiManager.startConfigMode();
    }
}

void loop() {
    debugConsole.loop();
    wifiManager.loop();
}
