#include "DebugConsole.h"

#include <Arduino.h>
#include "ESP8266WiFi.h"

#include "SerialCommands.h"
#include "DeviceSettings.h"

SerialCommands serialCommands;

void cmdHelp(void) {
    serialCommands.listCommands();
}

void cmdSerialNumberGet(void) {
	DeviceSettings::DeviceParameters params = DeviceSettings::getDeviceParameters();
	Serial.println(params.deviceSerialNumber);
}

void cmdSerialNumberSet(void) {
	DeviceSettings::DeviceParameters params;
	char* argument = serialCommands.getArgument();
	if (argument != NULL) {
        params.deviceSerialNumber = atoi(argument);
        DeviceSettings::setDeviceParameters(params);
    }
    else {
        Serial.println("Error - expecting argument 'serialNumber' as number!");
    }
}

void cmdEepromSave(void) {
	DeviceSettings::save();
}

void cmdWiFiScan(void) {
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.print("Scanning networks..");
    // WiFi.scanNetworks will return the number of networks found
    int networksFound = WiFi.scanNetworks();
    Serial.println("..done!");
    if (networksFound == 0) {
        Serial.println("No WiFi networks found!");
    }
    else {
        Serial.print(networksFound);
        Serial.println(" networks found:");
        for (int i = 0; i < networksFound; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");
}

DebugConsole::DebugConsole(void) {
}

void DebugConsole::setup(void) {
    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.SerialNumber.Get", cmdSerialNumberGet);
	serialCommands.addCommand("Config.SerialNumber.Set", cmdSerialNumberSet);
    serialCommands.addCommand("Config.Save", cmdEepromSave);
    serialCommands.addCommand("WiFi.Scan", cmdWiFiScan);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}