#include "SonosDiscovery.h"

std::vector<SonosDevice> SonosDiscovery::discover(const uint16_t timeoutMs) {
    std::vector<SonosDevice> deviceList;
    WiFiUDP udpClient;
    broadcastSearch(udpClient, "urn:schemas-upnp-org:device:ZonePlayer:1\r\n");
    
    // now wait the maximum defined time for all devices to respond
    unsigned long startTimeMs = millis();
    while ((millis() - startTimeMs) < timeoutMs) {
        processResponse(udpClient, deviceList);
        delay(10);
    }
    return deviceList;
}

SonosDevice SonosDiscovery::discoverByUID(const uint16_t timeoutMs, const std::string& uid) {
    std::vector<SonosDevice> deviceList;
    WiFiUDP udpClient;
    broadcastSearch(udpClient, "uuid:" + uid);
    
    // now wait the maximum defined time for the device to respond
    unsigned long startTimeMs = millis();
    while ((millis() - startTimeMs) < timeoutMs) {
        if (processResponse(udpClient, deviceList) && (deviceList.size() >= 1)) {
            // the device responded, immediatly return
            return deviceList[0];
        }
        delay(10);
    }
    // TODO: better error handling when no device found
    Serial.println("WARNING! SonosDiscovery - discoverByUID failed!");
    return SonosDevice();
}

SonosDevice SonosDiscovery::discoverCoordinator(const uint16_t timeoutMs, const std::string& roomName) {
    std::vector<SonosDevice> deviceList;
    WiFiUDP udpClient;
    broadcastSearch(udpClient, "urn:schemas-upnp-org:device:ZonePlayer:1\r\n");

    // wait the maximum defined time for all devices to respond, immediatly return once we have a matching coordinator
    // now wait the maximum defined time for the device to respond
    unsigned long startTimeMs = millis();
    while ((millis() - startTimeMs) < timeoutMs) {
        if (processResponse(udpClient, deviceList)) {
            // new device found
            SonosDevice foundDevice = deviceList[deviceList.size() - 1];
            if (foundDevice.getRoomName() == roomName) {
                if (foundDevice.isCoordinator()) {
                    return foundDevice;
                }
                else if (foundDevice.isJoined()) {
                    SonosZoneInfo zoneInfo = foundDevice.getZoneGroupState();
                    std::vector<SonosDevice> zoneDevices = zoneInfo.getSonosDevicesInGroup();
                    for (SonosDevice zoneDevice : zoneDevices) {
                        if (zoneDevice.isCoordinator(zoneInfo)) {
                            return zoneDevice;
                        }
                    }
                }
            }
            else {
                // remove from list, wait for the next device
                deviceList.pop_back();
            }
        }
    }
    // TODO: better error handling when no device found
    return SonosDevice();
}

void SonosDiscovery::broadcastSearch(WiFiUDP& udpClient, const std::string& serviceType) {
    udpClient.begin(SSDP_PORT);
    udpClient.beginPacketMulticast(IPAddress(239, 255, 255, 250), SSDP_PORT, WiFi.localIP());
    std::string request = "M-SEARCH * HTTP/1.1\r\n";
    request += "HOST: 239.255.255.250:1900\r\n";
    request += "MAN: \"ssdp:discover\"\r\n";
    request += "MX: 1\r\n";
    request += "ST: ";
    request += serviceType;
    request += "\r\n";
    udpClient.write(request.c_str());
    udpClient.endPacket();
}

bool SonosDiscovery::processResponse(WiFiUDP& udpClient, std::vector<SonosDevice>& deviceList) {
    int packetSize = udpClient.parsePacket();
    if (packetSize > 0) {
        // read the begin of the packet in order to extract the UUID
        char buffer[512];
        int bytesRead = udpClient.read(buffer, sizeof(buffer));
        if (bytesRead > 0) {
            // force null termination
            buffer[bytesRead] = 0;
            
            std::string uuidSearchToken = "USN: uuid:";
            std::string response(buffer);
            std::size_t uuidStartPos = response.find(uuidSearchToken);
            std::size_t uuidEndPos = response.find('\r', uuidStartPos);
            if ((uuidStartPos != std::string::npos) && (uuidEndPos != std::string::npos)) {
                // Response if discovered by UID = USN: uuid:RINCON_****
                // Response if discovered by Service = USN: uuid:RINCON_****::urn:schemas-upnp-org:device:ZonePlayer:1
                uuidStartPos += uuidSearchToken.length();
                std::string uuid = response.substr(uuidStartPos, uuidEndPos - uuidStartPos);
                std::size_t serviceDescriptionPos = uuid.find("::urn:schemas-upnp-org:device:ZonePlayer:1");
                if (serviceDescriptionPos != std::string::npos) {
                    uuid = uuid.substr(0, serviceDescriptionPos);
                }
                SonosDevice sonosDevice(udpClient.remoteIP(), uuid);

                // TODO: for a unknown reason we get sporadically multiple responses from the same device
                // check if device not already in the list
                bool deviceAdded = false;
                for (std::size_t i = 0; i < deviceList.size(); i++) {
                    if (deviceList[i].getIp() == udpClient.remoteIP()) {
                        deviceAdded = true;
                        break;
                    }
                }
                if (!deviceAdded) {
                    deviceList.push_back(sonosDevice);
                    return true;
                }
            }
        }
    }
    return false;
}