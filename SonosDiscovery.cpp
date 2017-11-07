#include "SonosDiscovery.h"

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

    // now wait the maximum defined time for all devices to respond
    unsigned long startTimeMs = millis();
    while((millis() - startTimeMs) < timeoutMs) {
        processResponse(udpClient, deviceList);
        delay(50);
    }
    return deviceList;
}

void SonosDiscovery::processResponse(WiFiUDP& udpClient, std::vector<SonosDevice>& deviceList) {
    int packetSize = udpClient.parsePacket();
    if(packetSize > 0) {
        // read the begin of the packet in order to extract the UUID
        char buffer[512];
        int bytesRead = udpClient.read(buffer, sizeof(buffer));
        if (bytesRead > 0) {
            // force null termination
            buffer[bytesRead] = 0;
            std::string response(buffer);
            std::size_t uuidStartPos = response.find("uuid:");
            std::size_t uuidEndPos = response.find("::urn:schemas-upnp-org:device:ZonePlayer:1");

            if ((uuidStartPos != std::string::npos) && (uuidEndPos != std::string::npos)) {
                // it's actually a Sonos device and it's safe to read the uuid
                uuidStartPos += 5; // advance start pointer by length of "uuid:"
                std::string uuid = response.substr(uuidStartPos, uuidEndPos - uuidStartPos);
                SonosDevice sonosDevice(udpClient.remoteIP(), uuid);
                
                // TODO: for a unknown reason we get sporadically multiple responses from the same device
                // check if device not already in the list
                bool deviceAdded = false;
                for(int i = 0; i < deviceList.size(); i++) {
                    if (deviceList[i].getUUID() == uuid) {
                        deviceAdded = true;
                        break;
                    }
                }
                if (!deviceAdded) {
                    deviceList.push_back(sonosDevice);
                }
            }
        }
    }
}