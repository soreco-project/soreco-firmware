#include "SonosDevice.h"

SonosDevice::SonosDevice(void) {
}

SonosDevice::SonosDevice(IPAddress ipAddress) :
    m_ipAddress(ipAddress) {
}

IPAddress SonosDevice::getIpAddress(void) {
    return m_ipAddress;
}