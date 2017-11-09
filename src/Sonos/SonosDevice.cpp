#include "SonosDevice.h"

#include <sstream>
#include "SonosCommandBuilder.h"

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

SonosDevice::PlayState SonosDevice::getPlayState(void) {
}

void SonosDevice::playUri(const std::string& uri, const std::string& meta) {
    SonosCommandBuilder::transport("SetAVTransportURI").put("InstanceID", "0").put("CurrentURI", uri).put("CurrentURIMetaData", meta).executeOn(m_ip, m_client);
    play();
}

void SonosDevice::play(void) {
    SonosCommandBuilder::transport("Play").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip, m_client);
}

void SonosDevice::pause(void) {
    SonosCommandBuilder::transport("Pause").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip, m_client);
}

void SonosDevice::next(void) {
    SonosCommandBuilder::transport("Next").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip, m_client);
}

void SonosDevice::previous(void) {
    SonosCommandBuilder::transport("Previous").put("InstanceID", "0").put("Speed", "1").executeOn(m_ip, m_client);
}

void SonosDevice::setVolume(int volume) {
    std::stringstream ss;
    ss << volume;
    SonosCommandBuilder::rendering("SetVolume").put("InstanceID", "0").put("Channel", "Master").put("DesiredVolume", ss.str()).executeOn(m_ip, m_client);
}
