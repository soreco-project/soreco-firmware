/*************************************************** 
 Helper to discover Sonos devices in network.
 ****************************************************/

#ifndef SONOSDISCOVERY_H
#define SONOSDISCOVERY_H

#include <vector>
#include "SonosDevice.h"

class SonosDiscovery {
public:

    /**
     * Discover all Sonos devices on network using SSDP (Simple Service Discovery Protocol).
     * @return List of Sonos devices
     * @throws IOException
     */
    static std::vector<SonosDevice> discover();

private:
    /**
     * Private default constructor.
     */
    SonosDiscovery(void);
    
    /**
     * Private copy constructor.
     */
    SonosDiscovery(const SonosDiscovery&);

    /**
     * Private assignment constructor.
     */
    SonosDiscovery& operator=(const SonosDiscovery&);
};

#endif // SONOSDISCOVERY_H