#include "SonosDevice.h"

SonosDevice::SonosDevice(IPAddress ipAddress, String uuid) :
    m_ipAddress(ipAddress),
    m_uuid(uuid) {
}

IPAddress SonosDevice::getIpAddress(void) {
    return m_ipAddress;
}

String SonosDevice::getUUID(void) {
    return m_uuid;
}