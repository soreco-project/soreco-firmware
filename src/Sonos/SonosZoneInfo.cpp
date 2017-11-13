#include "SonosZoneInfo.h"
#include "SonosDevice.h"
#include "SonosDiscovery.h"

SonosZoneInfo::SonosZoneInfo(const std::string& name, const std::string& id, const std::vector<std::string>& zonePlayerUIDInGroup) :
    m_name(name),
    m_id(id),
    m_zonePlayerUIDInGroup(zonePlayerUIDInGroup)
{
}

std::string SonosZoneInfo::getName(void) {
    return m_name;
}

std::string SonosZoneInfo::getId(void) {
    return m_id;
}

std::vector<std::string> SonosZoneInfo::getZonePlayerUIDInGroup(void) {
    return m_zonePlayerUIDInGroup;
}

std::vector<SonosDevice> SonosZoneInfo::getSonosDevicesInGroup(void) {
    std::vector<SonosDevice> devices;
    for (std::string uid : m_zonePlayerUIDInGroup) {
        SonosDevice device = SonosDiscovery::discoverByUID(1000, uid);
        devices.push_back(device);
    }
    return devices;
}