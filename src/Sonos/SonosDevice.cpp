#include "SonosDevice.h"

void onEthernetConnectionError(void) {
    // TODO
}

SonosDevice::SonosDevice(void) :
    m_sonosUPnP(onEthernetConnectionError)
{
}

SonosDevice::SonosDevice(IPAddress ipAddress, std::string uuid) :
    m_ipAddress(ipAddress),
    m_uuid(uuid),
    m_sonosUPnP(onEthernetConnectionError)
{
}

void SonosDevice::setIpAddress(IPAddress ipAddress) {
    m_ipAddress = ipAddress;
    // TODO: get UUID from device
}

IPAddress SonosDevice::getIpAddress(void) {
    return m_ipAddress;
}

std::string SonosDevice::getUUID(void) {
    return m_uuid;
}

SonosDevice::PlayState SonosDevice::getPlayState(void) {
    const uint8_t playState = m_sonosUPnP.getState(m_ipAddress);
    switch (playState) {
        case SONOS_STATE_ERROR:
            return SonosDevice::PlayState::ERROR;
        case SONOS_STATE_STOPPED:
            return SonosDevice::PlayState::STOPPED;
        case SONOS_STATE_PLAYING:
            return SonosDevice::PlayState::PLAYING;
        case SONOS_STATE_PAUSED:
            return SonosDevice::PlayState::PAUSED_PLAYBACK;
        case SONOS_STATE_TRANSITIONING:
            return SonosDevice::PlayState::TRANSITIONING;
        default:
            return SonosDevice::PlayState::STOPPED;
    }
}

void SonosDevice::playUri(std::string uri, std::string meta) {
    m_sonosUPnP.playUri(m_ipAddress, uri.c_str());
}


void SonosDevice::play(void) {
    m_sonosUPnP.play(m_ipAddress);
}

void SonosDevice::pause(void) {
    m_sonosUPnP.pause(m_ipAddress);
}
