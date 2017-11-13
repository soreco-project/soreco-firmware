#ifndef SONOSZONEINFO_H
#define SONOSZONEINFO_H

#include <string>
#include <vector>

// forward declaration
class SonosDevice;

class SonosZoneInfo {

public:

    SonosZoneInfo(const std::string& name, const std::string& id, const std::vector<std::string>& zonePlayerUIDInGroup);

    std::string getName(void);

    std::string getId(void);

    std::vector<std::string> getZonePlayerUIDInGroup(void);

    std::vector<SonosDevice> getSonosDevicesInGroup(void);

private:

    std::string m_name;
    std::string m_id;
    std::vector<std::string> m_zonePlayerUIDInGroup;
};

#endif // SONOSZONEINFO_H