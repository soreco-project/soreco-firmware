#include "DebugConsole.h"

#include <Arduino.h>
#include <string.h>
#include "SerialCommands.h"
#include "../DeviceSettings/DeviceSettings.h"
#include "../Sonos/SonosDiscovery.h"
#include "../Sonos/SonosCommandBuilder.h"

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
    const char* argument = serialCommands.getArgument();
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

void cmdConfigDeviceName(void) {
    DeviceSettings::DeviceConfig config = DeviceSettings::getDeviceConfig();
    const char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.println(config.deviceName);
    }
    else {
        // set
        strncpy(config.deviceName, argument, sizeof(config.deviceName));
        DeviceSettings::setDeviceConfig(config);
    }
}

void cmdConfigSonosRoom(void) {
    DeviceSettings::SonosConfig config = DeviceSettings::getSonosConfig();
    const char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.println(config.sonosRoom);
    }
    else {
        // set
        strncpy(config.sonosRoom, argument, sizeof(config.sonosRoom));
        DeviceSettings::setSonosConfig(config);
    }
}

void cmdConfigWiFiSSID(void) {
    DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
    const char* argument = serialCommands.getArgument();
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
    const char* argument = serialCommands.getArgument();
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
    const std::vector<WifiManager::WiFiNetwork> networks = pWiFiManager->scanForNetworks();
    Serial.print(F("..done! (")); Serial.print(networks.size()); Serial.println(F(" networks found)"));
    for (std::size_t i = 0; i < networks.size(); i++) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1); Serial.print(F(": ")); Serial.print(networks[i].ssid.c_str());
        Serial.print(F(" (")); Serial.print(networks[i].signalStrength); Serial.print(F(")"));
        Serial.println((networks[i].encryptionType == ENC_TYPE_NONE)? F(" ") : F("*"));
    }
}

void cmdWiFiConnect(void) {
    const DeviceSettings::WiFiConfig wifiConfig = DeviceSettings::getWiFiConfig();
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
    pWiFiManager->startConfigMode(DeviceSettings::getDeviceParameters().deviceSerialNumber);
}

void cmdWiFiStatusClient(void) {
    Serial.print(F("WiFi client mode: "));
    const wl_status_t wifiStatus = WiFi.status();
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
    const WiFiMode_t mode = WiFi.getMode();
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
    const char* argument = serialCommands.getArgument();
    uint16_t timeoutMs = 5000;
    if (argument != NULL) {
        timeoutMs = atoi(argument);
    }

    Serial.print(F("Discovering Sonos devices.."));
    std::vector<SonosDevice> sonosDevices = SonosDiscovery::discover(timeoutMs);
    Serial.print(F("..done! (")); Serial.print(sonosDevices.size()); Serial.println(F(" devices found)"));

    for (std::size_t i = 0; i < sonosDevices.size(); i++) {
        IPAddress ip = sonosDevices[i].getIp();
        const std::string uuid = sonosDevices[i].getUUID();
        const std::string roomName = sonosDevices[i].getRoomName();
        // cache the zone info to reduce network traffic
        const SonosZoneInfo zoneInfo = sonosDevices[i].getZoneGroupState();
        const bool isJoined = sonosDevices[i].isJoined(zoneInfo);
        const bool isCoordinator = sonosDevices[i].isCoordinator(zoneInfo);

        Serial.print(i + 1); Serial.print(F(": ")); Serial.print(roomName.c_str());
        Serial.print(F(" (")); Serial.print(ip); Serial.print(F(", ")); Serial.print(uuid.c_str());
        if (isJoined) {
            Serial.print(F(", joined"));
        }
        if (isCoordinator) {
            Serial.print(F(", coordinator"));
        }
        Serial.println(F(")"));
    }
}

void cmdSonosConnect(void) {
    const char* argument = serialCommands.getArgument();
    if (argument != NULL) {
        IPAddress ip;
        if (ip.fromString(argument)) {
            pSonosDevice->setIp(ip);
            return;
        }
    }
    Serial.println(F("expecting IP address as argument"));
}

void cmdSonosPlayState(void) {
    const char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        SonosDevice::PlayState::Id playState = pSonosDevice->getPlayState();
        Serial.print(F("Sonos play state = ")); Serial.println(SonosDevice::PlayState::toString(playState).c_str());
    }
    else {
        // set
        if (strcasecmp(argument, "play") == 0) {
            pSonosDevice->play();
        }
        else if (strcasecmp(argument, "pause") == 0) {
            pSonosDevice->pause();
        }
        else {
            Serial.println(F("unknown play state argument ('play' | 'pause')"));
        }
    }
}

void cmdSonosVolume(void) {
    const char* argument = serialCommands.getArgument();
    if (argument == NULL) {
        // get
        Serial.print(F("Sonos volume = ")); Serial.println(pSonosDevice->getVolume());
    }
    else {
        // set
        int volume = atoi(argument);
        pSonosDevice->setVolume(volume);
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
    serialCommands.addCommand("Config.DeviceName", cmdConfigDeviceName);
    serialCommands.addCommand("Config.Sonos.Room", cmdConfigSonosRoom);
    serialCommands.addCommand("Config.WiFi.SSID", cmdConfigWiFiSSID);
    serialCommands.addCommand("Config.WiFi.Passphrase", cmdConfigWiFiPassphrase);
    serialCommands.addCommand("WiFi.Scan", cmdWiFiScan);
    serialCommands.addCommand("WiFi.Connect", cmdWiFiConnect);
    serialCommands.addCommand("WiFi.StartHotspot", cmdWiFiStartHotspot);
    serialCommands.addCommand("WiFi.Status", cmdWiFiStatus);
    serialCommands.addCommand("Sonos.Discover", cmdSonosDiscover);
    serialCommands.addCommand("Sonos.Connect", cmdSonosConnect);
    serialCommands.addCommand("Sonos.PlayState", cmdSonosPlayState);
    serialCommands.addCommand("Sonos.Volume", cmdSonosVolume);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}