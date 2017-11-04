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

void cmdConfigSave(void) {
    DeviceSettings::save();
}

void cmdConfigClearAll(void) {
    DeviceSettings::clearAll();
}

void cmdConfigFactoryDefaults(void) {
    DeviceSettings::factoryDefaults();
}

void cmdConfigSerialNumber(void) {
    DeviceSettings::DeviceParameters config = DeviceSettings::getDeviceParameters();
    char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.println(config.deviceSerialNumber);
    }
    else {
        // set
        config.deviceSerialNumber = atoi(argument);
        DeviceSettings::setDeviceParameters(config);
    }
}

void cmdConfigWiFiSSID(void) {
    DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
    char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.println(config.ssid);
    }
    else {
        // set
        strncpy(config.ssid, argument, sizeof(config.ssid));
        DeviceSettings::setWiFiConfig(config);
    }
}

void cmdConfigWiFiPassphrase(void) {
    DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
    char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.println(config.passphrase);
    }
    else {
        // set
        strncpy(config.passphrase, argument, sizeof(config.passphrase));
        DeviceSettings::setWiFiConfig(config);
    }
}

void cmdWiFiScan(void) {
    Serial.print("Scanning WiFi networks..");
    std::vector<WifiManager::WiFiNetwork> networks = pWiFiManager->scanForNetworks();
    Serial.print("..done! ("); Serial.print(networks.size()); Serial.println(" networks found)");
    for (int i = 0; i < networks.size(); i++) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1); Serial.print(": "); Serial.print(networks[i].ssid);
        Serial.print(" ("); Serial.print(networks[i].signalStrength); Serial.print(")");
        Serial.println((networks[i].encryptionType == ENC_TYPE_NONE)?" ":"*");
    }
    Serial.println("");
}

void cmdWiFiConnect(void) {
    DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
    Serial.print("Connecting to WiFi network "); Serial.print(config.ssid);
    pWiFiManager->startClientMode(config.ssid, config.passphrase);

    int16_t timeOutMs = 5000;
    const int16_t delayMs = 500;
    while ((WiFi.status() != WL_CONNECTED) && (timeOutMs > 0)) {
        delay(delayMs);
        Serial.print(".");
        timeOutMs -= delayMs;
    }

    if (timeOutMs > 0) {
        Serial.println("success!");
        Serial.print("IP address: "); Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("failed!");
    }
}

void cmdWiFiStatus(void) {
    wl_status_t wifiStatus = WiFi.status();
    switch (wifiStatus) {
        case WL_CONNECTED:
            Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
            break;
        case WL_DISCONNECTED:
            Serial.println("WiFi disconnected");
            break;
        default:
            Serial.println(wifiStatus);
            break;
    }
}

DebugConsole::DebugConsole(void) {
}

DebugConsole::~DebugConsole(void) {
}

void DebugConsole::setup(WifiManager& wifiManager) {
    pWiFiManager = &wifiManager;

    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.Save", cmdConfigSave);
    serialCommands.addCommand("Config.ClearAll", cmdConfigClearAll);
    serialCommands.addCommand("Config.FactoryDefaults", cmdConfigFactoryDefaults);
    serialCommands.addCommand("Config.SerialNumber", cmdConfigSerialNumber);    
    serialCommands.addCommand("Config.WiFi.SSID", cmdConfigWiFiSSID);
    serialCommands.addCommand("Config.WiFi.Passphrase", cmdConfigWiFiPassphrase);
    serialCommands.addCommand("WiFi.Scan", cmdWiFiScan);
    serialCommands.addCommand("WiFi.Connect", cmdWiFiConnect);
    serialCommands.addCommand("WiFi.Status", cmdWiFiStatus);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}