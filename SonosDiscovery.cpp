#include "SonosDiscovery.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

std::vector<SonosDevice> SonosDiscovery::discover(const uint16_t timeoutMs) {
    std::vector<SonosDevice> deviceList;

    WiFiUDP udpClient;
    udpClient.begin(SSDP_PORT);

    udpClient.beginPacketMulticast(IPAddress(239, 255, 255, 250), SSDP_PORT, WiFi.localIP());
    udpClient.write("M-SEARCH * HTTP/1.1\r\n"
              "HOST: 239.255.255.250:1900\r\n"
              "MAN: \"ssdp:discover\"\r\n"
              "MX: 1\r\n"
              "ST: urn:schemas-upnp-org:device:ZonePlayer:1\r\n");
    udpClient.endPacket();

    uint64_t startTimeMs = millis();
    while((millis() - startTimeMs) < timeoutMs) {
        int packetSize = udpClient.parsePacket();
        if(packetSize > 0) {
            SonosDevice sonosDevice(udpClient.remoteIP());
            deviceList.push_back(sonosDevice);
        }
        delay(50);
    }
    return deviceList;
}