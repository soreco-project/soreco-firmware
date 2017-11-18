#ifndef SONOSDISCOVERY_H
#define SONOSDISCOVERY_H

#include <Arduino.h>
#include <vector>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

#include "SonosDevice.h"

/**
 * Helper class to discover all avaiable Sonos devices in the network.
 */
class SonosDiscovery {

public:

    /**
     * Discover all Sonos devices on network using SSDP (Simple Service Discovery Protocol).
     * @param timeOutMs discovery timeout in milliseconds
     * @return List of Sonos devices
     */
    static std::vector<SonosDevice> discover(const uint16_t timeoutMs);

    /**
     * Discover one Sonos speakers on network using SSDP (Simple Service Discovery Protocol) by UID.
     * @param timeOutMs discovery timeout in milliseconds
     * @param uid Sonos Speaker UID
     */
    static SonosDevice discoverByUID(const uint16_t timeoutMs, const std::string& uid);

    /**
     * Discover the Sonos speaker which is the coordinator using SSDP (Simple Service Discovery Protocol).
     * a) is at least a device with the given room name joined?
     *    --> if yes, find the coordinator of that group
     * b) there could be multiple speaker in the same room (e.g. stereo pair)
     *    --> if yes, find the coordinator of that room
     * @param timeOutMs discovery timeout in milliseconds
     * @param roomName name of the room to discover the coordinator
     */
    static SonosDevice discoverCoordinator(const uint16_t timeoutMs, const std::string& roomName);

private:

    // SSDP port (Simple Service Discovery Protocol)
    static const uint16_t SSDP_PORT = 1900;

    // private helpers
    static void broadcastSearch(WiFiUDP& udpClient, const std::string& serviceType);
    static bool processResponse(WiFiUDP& udpClient, std::vector<SonosDevice>& deviceList);

    /**
     * Private default constructor.
     */
    SonosDiscovery(void);

    /**
     * Private copy constructor.
     */
    SonosDiscovery(const SonosDiscovery&);

    /**
     * Private assignment operator.
     */
    SonosDiscovery& operator=(const SonosDiscovery&);
};

#endif // SONOSDISCOVERY_H
