#include "SonosDevice.h"

#include "SonosCommandBuilder.h"
#include "SonosResponseParser.h"

// static initialization
std::unordered_map<std::string, SonosDevice::PlayState::Id> SonosDevice::PlayState::s_valueMap {
    {"ERROR", PlayState::Id::ERROR}, {"STOPPED", PlayState::Id::STOPPED},
    {"PLAYING", PlayState::Id::PLAYING}, {"PAUSED_PLAYBACK", PlayState::Id::PAUSED_PLAYBACK},
    {"TRANSITIONING", PlayState::Id::TRANSITIONING}};

SonosDevice::PlayState::Id SonosDevice::PlayState::valueOf(const std::string& s) {
    return s_valueMap.at(s);
}

std::string SonosDevice::PlayState::toString(const PlayState::Id value) {
    for (auto it = s_valueMap.begin(); it != s_valueMap.end(); it++) {
        if (it->second == value) {
            return it->first;
        }
    }
    return "";
}

SonosDevice::SonosDevice(void) {
}

SonosDevice::SonosDevice(const IPAddress& ip, const std::string& uuid) :
    m_ip(ip),
    m_uuid(uuid) {
}

bool SonosDevice::isIpValid(void) {
    return m_ip != 0;
}

void SonosDevice::setIp(const IPAddress& ip) {
    m_ip = ip;
    updateUUID();
}

IPAddress SonosDevice::getIp(void) const {
    return m_ip;
}

std::string SonosDevice::getUUID(void) {
    if (!isUUIDValid()) {
        updateUUID();
    }
    return m_uuid;
}

SonosDevice::PlayState::Id SonosDevice::getPlayState(void) const {
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

uint8_t SonosDevice::getVolume(void) const {
    std::string r = SonosCommandBuilder::rendering("GetVolume").put("InstanceID", "0").put("Channel", "Master").executeOn(m_ip);
    std::string volume = SonosResponseParser::findOne("<CurrentVolume>", "</CurrentVolume>", r);
    // note: we cannot use std::stringstream in Arduino since it will create linker errors
    return String(volume.c_str()).toInt();
}

void SonosDevice::setVolume(uint8_t volume) {
    // note: we cannot use std::stringstream in Arduino since it will create linker errors
    std::string s = String(volume).c_str();
    SonosCommandBuilder::rendering("SetVolume").put("InstanceID", "0").put("Channel", "Master").put("DesiredVolume", s).executeOn(m_ip);
}

SonosZoneInfo SonosDevice::getZoneGroupState(void) const {
    std::string r = SonosCommandBuilder::zoneGroupTopology("GetZoneGroupAttributes").executeOn(m_ip);
    std::string name = SonosResponseParser::findOne("<CurrentZoneGroupName>", "</CurrentZoneGroupName>", r);
    std::string id = SonosResponseParser::findOne("<CurrentZoneGroupID>", "</CurrentZoneGroupID>", r);
    std::string devices = SonosResponseParser::findOne("<CurrentZonePlayerUUIDsInGroup>", "</CurrentZonePlayerUUIDsInGroup>", r);

    // split devices
    std::vector<std::string> deviceList = SonosResponseParser::split(devices, ',');
    return SonosZoneInfo(name, id, deviceList);
}

std::string SonosDevice::getRoomName(void) const {
    return getRoomName(SonosCommandBuilder::getDeviceDescription(m_ip));
}

std::string SonosDevice::getRoomName(const std::string& xmlDeviceDescription) {
    return SonosResponseParser::findOne("<roomName>", "</roomName>", xmlDeviceDescription);
}

bool SonosDevice::isJoined(void) const {
    return isJoined(getZoneGroupState());
}

bool SonosDevice::isJoined(const SonosZoneInfo& sonosZoneInfo) {
    return sonosZoneInfo.getZonePlayerUIDInGroup().size() > 1;
}

bool SonosDevice::isCoordinator(void) {
    return isCoordinator(getZoneGroupState());
}

bool SonosDevice::isCoordinator(const SonosZoneInfo& sonosZoneInfo) {
    // If zone starts with the same UUID as the speaker -> speaker is the coordinator of the zone
    std::string uid = getUUID();
    return sonosZoneInfo.getId().compare(0, uid.length(), uid) == 0;
}

bool SonosDevice::isUUIDValid(void) {
    return m_uuid.length() > 0;
}

void SonosDevice::updateUUID(void) {
    std::string r = SonosCommandBuilder::getDeviceDescription(m_ip);
    m_uuid = SonosResponseParser::findOne("<UDN>", "</UDN>", r).substr(strlen("uuid:"));
}