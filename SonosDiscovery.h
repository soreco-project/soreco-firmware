/*************************************************** 
 Helper to discover Sonos devices in network.
 ****************************************************/

#ifndef SONOSDISCOVERY_H
#define SONOSDISCOVERY_H

#include <vector>
#include "SonosDevice.h"

class SonosDisovery {
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
    SonosDisovery(void);
    
    /**
     * Private copy constructor.
     */
    SonosDisovery(const SonosDisovery&);

    /**
     * Private assignment constructor.
     */
    SonosDisovery& operator=(const SonosDisovery&);
};

#endif // SONOSDISCOVERY_H