#include <Arduino.h>
#include "WifiManager.h"
#include <ESP8266WiFi.h>


const IPAddress localIp(192, 168, 4, 1);
const IPAddress gateway(192, 168, 4, 1);
const IPAddress subnet(255, 255, 255, 0);

WifiManager::WifiManager(void) {
}

WifiManager::~WifiManager(void) {
}

void WifiManager::setup(void) {
    // do not save WiFi configuration in flash, we have our own EEPROM settings for that
    WiFi.persistent(false);
}

bool WifiManager::isWifiConnected(void) const {
    return WiFi.status() == WL_CONNECTED;
}

void WifiManager::startConfigMode(const uint32_t deviceSerialNumber) {
    // Intentionally disconnect so that ESP8266 knows clearly what we do and stops unrelated previous operations
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    // TODO: analyze why is the delay required..
    delay(100);

    WiFi.mode(WIFI_AP);

    char ssid[32+1];
    memcpy(ssid, CONFIG_SSID_PREFIX, strlen(CONFIG_SSID_PREFIX));
    itoa(deviceSerialNumber, &ssid[strlen(CONFIG_SSID_PREFIX)], 10);

    // start config hotspot with device serial number as ssid (no password)
    WiFi.softAPConfig(localIp, gateway, subnet);
    bool success = WiFi.softAP(ssid);
    // TODO configure WiFi.softAP(ssid, password, channel, hidden) for WPA2-PSK

    // TODO: try to not write to serial console within the components
    if (success) {
        Serial.print(F("Hotspot ")); Serial.print(ssid); Serial.println(F(" started"));
        Serial.print(F("soreco host address: ")); Serial.println(WiFi.softAPIP());
    }
    else {
        Serial.println(F("Failed to start hotspot!"));
    }
}

void WifiManager::startClientMode(const char* ssid, const char* passphrase) {
    // Intentionally disconnect so that ESP8266 knows clearly what we do and stops unrelated previous operations
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    // TODO: analyze why is the delay required..
    delay(100);

    // Explicitly set the ESP8266 to be a WiFi-client, otherwise it would try to act as both a client and an access-point
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
    // wait for connection
    WiFi.waitForConnectResult();
    
    // TODO: investigate modem-sleep to reduce power consumption in station mode
}

std::vector<WifiManager::WiFiNetwork> WifiManager::scanForNetworks(void) {
    std::vector<WiFiNetwork> networkList;
    int networksFound = WiFi.scanNetworks();
    for (int i = 0; i < networksFound; i++) {
        // convert Arduino String to std::string
        String ssid = WiFi.SSID(i);
        char ssidCharBuffer[ssid.length()+1];
        ssid.toCharArray(ssidCharBuffer, sizeof(ssidCharBuffer));

        WiFiNetwork network;
        network.ssid = ssidCharBuffer;
        network.signalStrength = WiFi.RSSI(i);
        network.encryptionType = (wl_enc_type)WiFi.encryptionType(i);
        networkList.push_back(network);
    }
    return networkList;
}
