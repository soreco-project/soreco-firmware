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

SonosDevice::SonosDevice(const IPAddress& ip, const std::string& uuid) :
    m_ip(ip),
    m_uuid(uuid)
{
}

void SonosDevice::setIp(const IPAddress& ip) {
    m_ip = ip;
    // TODO: get UUID from device
}

IPAddress SonosDevice::getIp(void) {
    return m_ip;
}

std::string SonosDevice::getUUID(void) {
    return m_uuid;
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
