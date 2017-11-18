#ifndef SONOSZONEINFO_H
#define SONOSZONEINFO_H

#include <string>
#include <vector>

// forward declaration
class SonosDevice;

class SonosZoneInfo {

public:

    /**
     * Constructor
     */
    SonosZoneInfo(const std::string& name, const std::string& id, const std::vector<std::string>& zonePlayerUIDInGroup);

    /**
     * Get the name of the Zone
     */
    std::string getName(void) const;

    /**
     * Get the identifier of the Zone (as UID)
     */
    std::string getId(void) const;

    /**
     * Get the list of UID players grouped in this zone.
     */
    std::vector<std::string> getZonePlayerUIDInGroup(void) const;

    /**
     * Get the list of devices grouped in this zone.
     * Note: this will discover each device and generates network traffic
     */
    std::vector<SonosDevice> getSonosDevicesInGroup(void) const;

private:

    std::string m_name;
    std::string m_id;
    std::vector<std::string> m_zonePlayerUIDInGroup;
};

#endif // SONOSZONEINFO_H