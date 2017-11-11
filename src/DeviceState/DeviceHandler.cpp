#include "DeviceHandler.h"


DeviceHandler::DeviceHandler(WifiManager& wifiManager, SonosDevice& sonosDevice) :
	m_wiFiManager(wifiManager),
	m_sonosDevice(sonosDevice) {

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