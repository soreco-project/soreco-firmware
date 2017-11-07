#include "DebugConsole.h"

#include <Arduino.h>

#include "SerialCommands.h"
#include "DeviceSettings.h"
#include "SonosDiscovery.h"

// Note: try to use flash strings to reduce RAM usage!
// https://espressif.com/sites/default/files/documentation/save_esp8266ex_ram_with_progmem_en.pdf

SerialCommands serialCommands;
// workaround to have access from non-member functions to components
WifiManager* pWiFiManager = NULL;
SonosDevice* pSonosDevice = NULL;

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
    Serial.print(F("Scanning WiFi networks.."));
    std::vector<WifiManager::WiFiNetwork> networks = pWiFiManager->scanForNetworks();
    Serial.print(F("..done! (")); Serial.print(networks.size()); Serial.println(F(" networks found)"));
    for (int i = 0; i < networks.size(); i++) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1); Serial.print(F(": ")); Serial.print(networks[i].ssid.c_str());
        Serial.print(F(" (")); Serial.print(networks[i].signalStrength); Serial.print(F(")"));
        Serial.println((networks[i].encryptionType == ENC_TYPE_NONE)? F(" ") : F("*"));
    }
}

void cmdWiFiConnect(void) {
    DeviceSettings::WiFiConfig wifiConfig = DeviceSettings::getWiFiConfig();
    Serial.print(F("Connecting to configured WiFi ")); Serial.println(wifiConfig.ssid);
    pWiFiManager->startClientMode(wifiConfig.ssid, wifiConfig.passphrase);

    int16_t timeOutMs = 15000;
    const int16_t delayMs = 1000;
    while ((WiFi.status() != WL_CONNECTED) && (timeOutMs > 0)) {
        delay(delayMs);
        Serial.print(F("."));
        timeOutMs -= delayMs;
    }

    if (timeOutMs > 0) {
        Serial.println(F("success!"));
        Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
    }
    else {
        Serial.println(F("failed!"));
    }
}

void cmdWiFiStartHotspot(void) {
    Serial.println(F("Starting WiFi hotspot for configuration"));
    pWiFiManager->startConfigMode();
}

void cmdWiFiStatusClient(void) {
    Serial.print(F("WiFi client mode: "));
    wl_status_t wifiStatus = WiFi.status();
    switch (wifiStatus) {
        case WL_CONNECTED:
            Serial.print(F("connected to ")); Serial.println(WiFi.SSID());
            Serial.print(F("IP: ")); Serial.println(WiFi.localIP());
            Serial.print(F("Signal strength: ")); Serial.println(WiFi.RSSI());
            break;
        case WL_DISCONNECTED:
            Serial.println(F("disconnected"));
            break;
        default:
            Serial.println(wifiStatus);
            break;
    }
}

void cmdWiFiStatusConfiguration(void) {
    Serial.print(F("WiFi configuration mode: "));
    Serial.print(WiFi.softAPgetStationNum()); Serial.println(F(" stations connected"));
}

void cmdWiFiStatus(void) {
    WiFiMode_t mode = WiFi.getMode();
    switch(mode) {
        case WIFI_AP:
            cmdWiFiStatusConfiguration();
            break;
        case WIFI_STA:
            cmdWiFiStatusClient();
            break;
        default:
            Serial.println(F("Unknown WiFi mode"));
            break;
    }
}

void cmdSonosDiscover(void) {
    uint16_t timeoutMs = 5000;
    char* argument = serialCommands.getArgument();
    if (argument != NULL) {
        timeoutMs = atoi(argument);
    }

    Serial.print(F("Discovering Sonos devices.."));
    std::vector<SonosDevice> sonosDevices = SonosDiscovery::discover(timeoutMs);
    Serial.print(F("..done! (")); Serial.print(sonosDevices.size()); Serial.println(F(" devices found)"));

    for (int i = 0; i < sonosDevices.size(); i++) {
        Serial.print(i + 1); Serial.print(F(": ")); Serial.print(sonosDevices[i].getIpAddress());
        Serial.print(F(" (")); Serial.print(sonosDevices[i].getUUID().c_str()); Serial.println(F(")"));
    }
}

void cmdSonosConnect(void) {
    char* argument = serialCommands.getArgument();
    if (argument != NULL) {
        IPAddress addr;
        if (addr.fromString(argument)) {
            pSonosDevice->setIpAddress(addr);
            return;
        }
    }
    Serial.println(F("expecting IP address as argument"));
}

void cmdSonosPlayState(void) {
    char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        SonosDevice::PlayState playState = pSonosDevice->getPlayState();
        Serial.print(F("Play state = ")); 
        switch(playState) {
            case SonosDevice::PlayState::ERROR:
                Serial.println(F("error"));
                break;
            case SonosDevice::PlayState::STOPPED:
                Serial.println(F("stopped"));
                break;
            case SonosDevice::PlayState::PLAYING:
                Serial.println(F("playing"));
                break;
            case SonosDevice::PlayState::PAUSED_PLAYBACK:
                Serial.println(F("paused"));
                break;
            default:
                Serial.println(playState);
                break;
        }
    }
    else {
        // set
        if (stricmp(argument, "play") == 0) {
            pSonosDevice->play();
        }
        else if (stricmp(argument, "pause") == 0) {
            pSonosDevice->pause();
        }
        else {
            Serial.println(F("unknown play state argument ('play' | 'pause')"));
        }
    }
}

DebugConsole::DebugConsole(void) {
}

DebugConsole::~DebugConsole(void) {
}

void DebugConsole::setup(WifiManager& wifiManager, SonosDevice& sonosDevice) {
    pWiFiManager = &wifiManager;
    pSonosDevice = &sonosDevice;

    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.Save", cmdConfigSave);
    serialCommands.addCommand("Config.ClearAll", cmdConfigClearAll);
    serialCommands.addCommand("Config.FactoryDefaults", cmdConfigFactoryDefaults);
    serialCommands.addCommand("Config.SerialNumber", cmdConfigSerialNumber);    
    serialCommands.addCommand("Config.WiFi.SSID", cmdConfigWiFiSSID);
    serialCommands.addCommand("Config.WiFi.Passphrase", cmdConfigWiFiPassphrase);
    serialCommands.addCommand("WiFi.Scan", cmdWiFiScan);
    serialCommands.addCommand("WiFi.Connect", cmdWiFiConnect);
    serialCommands.addCommand("WiFi.StartHotspot", cmdWiFiStartHotspot);
    serialCommands.addCommand("WiFi.Status", cmdWiFiStatus);
    serialCommands.addCommand("Sonos.Discover", cmdSonosDiscover);
    serialCommands.addCommand("Sonos.Connect", cmdSonosConnect);
    serialCommands.addCommand("Sonos.PlayState", cmdSonosPlayState);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}