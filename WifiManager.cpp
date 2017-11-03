#include <Arduino.h>
#include "WifiManager.h"


const IPAddress localIp(192, 168, 4, 1);
const IPAddress gateway(192, 168, 4, 1);
const IPAddress subnet(255, 255, 255, 0);


WifiManager::WifiManager(void) :
    m_server(WifiManager::PORT) {
}

void WifiManager::startServiceMode(void) {
    // disconnect wifi from an AP
    WiFi.disconnect();

    WiFi.mode(WIFI_AP);    
    WiFi.softAPConfig(localIp, gateway, subnet);
    // start accesspoint with ssid and password on channel 1
    WiFi.softAP(SSID, PASSWORD, 1, SSID_HIDDEN);
    
    Serial.println("AP started...");
    Serial.print("Soreco host address: "); Serial.println(WiFi.softAPIP());
}

void WifiManager::scanForNetworks(void) {
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
