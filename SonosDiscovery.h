#ifndef SONOSDISCOVERY_H
#define SONOSDISCOVERY_H

#include <vector>
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

private:

    // Simple Service Discovery Protocol
    static const uint16_t SSDP_PORT = 1900;

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