#include "DeviceHandler.h"
#include "../Sonos/SonosDiscovery.h"

DeviceHandler::DeviceHandler(WifiManager& wifiManager, SonosDevice& sonosDevice) :
    m_wiFiManager(wifiManager),
    m_sonosCoordinator(sonosDevice),
    m_sonosConnected(false) {
}

DeviceHandler::~DeviceHandler(void) {
}

bool DeviceHandler::isWifiConfigured(void) const {
    return DeviceSettings::getWiFiConfig().isConfigured();
}

bool DeviceHandler::isWifiConnected(void) const {
    return m_wiFiManager.isWifiConnected();
}

void DeviceHandler::startWifi(void) {
    const DeviceSettings::WiFiConfig wifiConfig = DeviceSettings::getWiFiConfig();
    Serial.print(F("Connecting to configured WiFi ")); Serial.println(wifiConfig.ssid);
    m_wiFiManager.startClientMode(wifiConfig.ssid, wifiConfig.passphrase);
}

void DeviceHandler::startHotspot(void) {
    const DeviceSettings::DeviceParameters deviceConfig = DeviceSettings::getDeviceParameters();
    Serial.println(F("Starting WiFi hotspot for configuration"));
    m_wiFiManager.startConfigMode(deviceConfig.deviceSerialNumber);
}

bool DeviceHandler::isSonosConfigured(void) const {
    return DeviceSettings::getSonosConfig().isConfigured();
}

void DeviceHandler::connectToSonos(void) {
    const DeviceSettings::SonosConfig sonosConfig = DeviceSettings::getSonosConfig();
    std::string roomName(sonosConfig.sonosRoom);
    if (roomName.empty()) {
        Serial.println(F("Warning - no Sonos room configured"));
        return;
    }

    Serial.print(F("Connecting to Sonos room ")); Serial.println(roomName.c_str());
    const uint16_t SONOS_DISCOVER_TIMEOUT_MS = 1000;
    SonosDevice coordinator = SonosDiscovery::discoverCoordinator(SONOS_DISCOVER_TIMEOUT_MS, roomName);
    if (coordinator.isIpValid()) {
        Serial.print("..found Sonos coordinator with IP "); Serial.println(coordinator.getIp());
        setSonosCoordinator(coordinator);
    }
    else {
        Serial.print(F("Warning - unable to connect to Sonos room ")); Serial.println(roomName.c_str());
    }
}

bool DeviceHandler::isSonosConnected(void) const {
    // TODO: check connection to Sonos (or return true if no room configured)
    return m_sonosConnected;
}

void DeviceHandler::onEventVolumeUp(uint16_t volumeStepCount) {
}

void DeviceHandler::onEventVolumeDown(uint16_t volumeStepCount) {
}

void DeviceHandler::onEventPlayPause(void) {
}

void DeviceHandler::onEventNext(void) {
}

void DeviceHandler::onEventPrevious(void) {
}

void DeviceHandler::onEventPreset1(void) {
}

void DeviceHandler::onEventPreset2(void) {
}

void DeviceHandler::onEventPreset3(void) {
}

void DeviceHandler::onEventPreset4(void) {
}

void DeviceHandler::onEventConfigMode(void) {
}

void DeviceHandler::onEventRestart(void) {
}

void DeviceHandler::setSonosCoordinator(SonosDevice& sonosCoordinator) {
    m_sonosCoordinator = sonosCoordinator;
    m_sonosConnected = true;
}