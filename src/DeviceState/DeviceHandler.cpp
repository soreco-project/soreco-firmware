#include "DeviceHandler.h"
#include "../Sonos/SonosDiscovery.h"

DeviceHandler::DeviceHandler(WifiManager& wifiManager, SonosDevice& sonosDevice) :
    m_wiFiManager(wifiManager),
    m_sonosCoordinator(sonosDevice),
    m_sonosConnected(false) {
}

DeviceHandler::~DeviceHandler(void) {
}

void DeviceHandler::startWifi(const DeviceSettings::WiFiConfig& wifiConfig) {
    Serial.print(F("Connecting to configured WiFi ")); Serial.println(wifiConfig.ssid);
    m_wiFiManager.startClientMode(wifiConfig.ssid, wifiConfig.passphrase);
}

bool DeviceHandler::isWifiConnected(void) const {
    return m_wiFiManager.isWifiConnected();
}

void DeviceHandler::startHotspot(const DeviceSettings::DeviceParameters& deviceConfig) {
    Serial.println(F("Starting WiFi hotspot for configuration"));
    m_wiFiManager.startConfigMode(deviceConfig.deviceSerialNumber);
}

void DeviceHandler::connectToSonos(const DeviceSettings::SonosConfig& sonosConfig) {
    std::string configuredRoomName(sonosConfig.sonosRoom);
    if (configuredRoomName.empty()) {
        Serial.println(F("Warning - no Sonos room configured"));
        return;
    }
    Serial.print(F("Connecting to configured Sonos ")); Serial.println(sonosConfig.sonosRoom);

    const uint16_t SONOS_DISCOVER_TIMEOUT_MS = 5000;
    std::vector<SonosDevice> devices = SonosDiscovery::discover(SONOS_DISCOVER_TIMEOUT_MS);

    // now filter with the following criteria:
    // a) is at least a device with the given room name joined?
    //    --> if yes, find the coordinator of that group
    // b) there could be multiple speaker in the same room (e.g. stereo pair)
    //    --> if yes, find the coordinator of that room
    for(SonosDevice sonosDevice : devices) {
        if (sonosDevice.getRoomName() == configuredRoomName) {
            if (sonosDevice.isCoordinator()) {
                setSonosCoordinator(sonosDevice);
                Serial.print("Found Sonos room coordinator = "); Serial.println(m_sonosCoordinator.getIp());
                return;
            }
            else if (sonosDevice.isJoined()) {
                std::vector<SonosDevice> zoneDevices = sonosDevice.getZoneGroupState().getSonosDevicesInGroup();
                for(SonosDevice zoneDevice : zoneDevices) {
                    if (zoneDevice.isCoordinator()) {
                        setSonosCoordinator(sonosDevice);
                        Serial.print("Found Sonos zone coordinator = "); Serial.println(m_sonosCoordinator.getIp());
                        return;
                    }
                }
            }
        }
    }
    Serial.println(F("Warning - unable to connect to configured Sonos room"));
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