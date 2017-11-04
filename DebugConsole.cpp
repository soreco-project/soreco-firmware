#include "DebugConsole.h"

#include <Arduino.h>

#include "SerialCommands.h"
#include "DeviceSettings.h"

SerialCommands serialCommands;
// workaround to have access from non-member functions to components
WifiManager* pWiFiManager = NULL;

void cmdHelp(void) {
    serialCommands.listCommands();
}

void cmdConfigSerialNumberGet(void) {
    DeviceSettings::DeviceParameters params = DeviceSettings::getDeviceParameters();
    Serial.println(params.deviceSerialNumber);
}

void cmdConfigSerialNumberSet(void) {
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

void cmdConfigSave(void) {
    DeviceSettings::save();
}

void cmdConfigClearAll(void) {
    DeviceSettings::clearAll();
}

void cmdConfigFactoryDefaults(void) {
    DeviceSettings::factoryDefaults();
}

void cmdWiFiScan(void) {
    Serial.print("Scanning networks..");
    std::vector<WifiManager::WiFiNetwork> networks = pWiFiManager->scanForNetworks();
    Serial.print("..done! ("); Serial.print(networks.size()); Serial.println(" networks found)");
    for (int i = 0; i < networks.size(); i++) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(networks[i].ssid);
        Serial.print(" (");
        Serial.print(networks[i].signalStrength);
        Serial.print(")");
        Serial.println((networks[i].encryptionType == ENC_TYPE_NONE)?" ":"*");
    }
    Serial.println("");
}

DebugConsole::DebugConsole(void) {
}

DebugConsole::~DebugConsole(void) {
}

void DebugConsole::setup(WifiManager& wifiManager) {
    pWiFiManager = &wifiManager;

    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.SerialNumber.Get", cmdConfigSerialNumberGet);
    serialCommands.addCommand("Config.SerialNumber.Set", cmdConfigSerialNumberSet);
    serialCommands.addCommand("Config.Save", cmdConfigSave);
    serialCommands.addCommand("Config.ClearAll", cmdConfigClearAll);
    serialCommands.addCommand("Config.FactoryDefaults", cmdConfigFactoryDefaults);
    serialCommands.addCommand("WiFi.Scan", cmdWiFiScan);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}