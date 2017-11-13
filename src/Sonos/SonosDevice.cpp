#include "SonosDevice.h"

#include <sstream>
#include "SonosCommandBuilder.h"
#include "SonosResponseParser.h"

// static initialization
std::unordered_map<std::string, SonosDevice::PlayState::Id> SonosDevice::PlayState::s_valueMap {
    {"ERROR", PlayState::Id::ERROR}, {"STOPPED", PlayState::Id::STOPPED},
    {"PLAYING", PlayState::Id::PLAYING}, {"PAUSED_PLAYBACK", PlayState::Id::PAUSED_PLAYBACK},
    {"TRANSITIONING", PlayState::Id::TRANSITIONING}};

SonosDevice::SonosDevice(void) {
}

SonosDevice::SonosDevice(const IPAddress& ip) :
    m_ip(ip)
{
}

void SonosDevice::setIp(const IPAddress& ip) {
    m_ip = ip;
}

IPAddress SonosDevice::getIp(void) {
    return m_ip;
}

SonosDevice::PlayState::Id SonosDevice::getPlayState(void) {
    std::string r = SonosCommandBuilder::transport("GetTransportInfo").put("InstanceID", "0").executeOn(m_ip);
    return PlayState::valueOf(SonosResponseParser::findOne("<CurrentTransportState>", "</CurrentTransportState>", r));
}

void SonosDevice::playUri(const std::string& uri, const std::string& meta) {
    SonosCommandBuilder::transport("SetAVTransportURI").put("InstanceID", "0").put("CurrentURI", uri).put("CurrentURIMetaData", meta).executeOn(m_ip);
    play();
}

void SonosDevice::play(void) {
    SonosCommandBuilder::transport("Play").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip);
}

void SonosDevice::pause(void) {
    SonosCommandBuilder::transport("Pause").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip);
}

void SonosDevice::next(void) {
    SonosCommandBuilder::transport("Next").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip);
}

void SonosDevice::previous(void) {
    SonosCommandBuilder::transport("Previous").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip);
}

void SonosDevice::setVolume(int volume) {
    std::stringstream ss;
    ss << volume;
    SonosCommandBuilder::rendering("SetVolume").put("InstanceID", "0").put("Channel", "Master").put("DesiredVolume", ss.str()).executeOn(m_ip);
}

std::string SonosDevice::getRoomName(void) {
    std::string r = SonosCommandBuilder::getDeviceDescription(m_ip);
    std::string name = SonosResponseParser::findOne("<roomName>", "</roomName>", r);
    return name;
}

bool SonosDevice::isJoined(void) {
    return getZoneGroupState().getZonePlayerUIDInGroup().size() > 1;
}

SonosZoneInfo SonosDevice::getZoneGroupState(void) {
    std::string r = SonosCommandBuilder::zoneGroupTopology("GetZoneGroupAttributes").executeOn(m_ip);
    std::string name = SonosResponseParser::findOne("<CurrentZoneGroupName>", "</CurrentZoneGroupName>", r);
    std::string id = SonosResponseParser::findOne("<CurrentZoneGroupID>", "</CurrentZoneGroupID>", r);
    std::string devices = SonosResponseParser::findOne("<CurrentZonePlayerUUIDsInGroup>", "</CurrentZonePlayerUUIDsInGroup>", r);

    // split devices
    std::string device;
    std::vector<std::string> deviceList;
    std::size_t p = 0;
    std::size_t q;
    while ((q = devices.find(',', p)) != std::string::npos) {
        deviceList.emplace_back(devices, p, q - p);
        p = q + 1;
    }
    deviceList.emplace_back(devices, p);
    return SonosZoneInfo(name, id, deviceList);
}

bool SonosDevice::isCoordinator(void) {
    // If zone have the same UID as the speaker -> speaker is the coordinator of the zone.
    std::string r = SonosCommandBuilder::getDeviceDescription(m_ip);
    std::string uid = SonosResponseParser::findOne("<UDN>", "</UDN>", r);
    uid = uid.substr(strlen("uuid:"));
    SonosZoneInfo zoneInfo = getZoneGroupState();
    return zoneInfo.getId().compare(0, uid.length(), uid) == 0;
}
