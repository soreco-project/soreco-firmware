#include <Arduino.h>
#include "WifiManager.h"
#include "DeviceSettings.h"

const IPAddress localIp(192, 168, 4, 1);
const IPAddress gateway(192, 168, 4, 1);
const IPAddress subnet(255, 255, 255, 0);

WifiManager::WifiManager(void) :
    m_server(WifiManager::PORT) {
}

WifiManager::~WifiManager(void) {
}

void WifiManager::setup(void) {
    // do not save WiFi configuration in flash, we have our own EEPROM settings for that
    WiFi.persistent(false);
}

void WifiManager::loop(void) {
  WiFiClient client = m_server.available();
  // wait for a client (web browser) to connect
  if (client) {
        if (client.connected()) {
            Serial.println("Connected");
            // TODO parse data
            // client.readBytesUntil(char terminator, char *buffer, size_t length)
        }
        client.stop();
    }
}

void WifiManager::startConfigMode(void) {
    // disconnect wifi from an AP
    WiFi.disconnect();

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(localIp, gateway, subnet);
    
    char ssid[32+1];
    memcpy(ssid, CONFIG_SSID_PREFIX, strlen(CONFIG_SSID_PREFIX));
    itoa(DeviceSettings::getDeviceParameters().deviceSerialNumber, &ssid[strlen(CONFIG_SSID_PREFIX)], 10);
    // start config hotspot with device serial number as ssid (no password)
    WiFi.softAP(ssid);
    
    Serial.println("AP started...");
    Serial.print("soreco host address: "); Serial.println(WiFi.softAPIP());
}

void WifiManager::startClientMode(char* ssid, char* passphrase) {
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise it would try to act as both a client and an access-point */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
}

std::vector<WifiManager::WiFiNetwork> WifiManager::scanForNetworks(void) {
    std::vector<WiFiNetwork> networkList;
    int networksFound = WiFi.scanNetworks();
    for (int i = 0; i < networksFound; i++) {
        WiFiNetwork network;
        network.ssid = WiFi.SSID(i);
        network.signalStrength = WiFi.RSSI(i);
        network.encryptionType = (wl_enc_type)WiFi.encryptionType(i);
        networkList.push_back(network);
    }
    return networkList;
}
