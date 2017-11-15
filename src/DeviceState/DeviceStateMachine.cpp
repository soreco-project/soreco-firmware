#include "DeviceStateMachine.h"

#include <Arduino.h>
#include "../DeviceSettings/DeviceSettings.h"
#include "../WifiManager/WifiManager.h"
#include "../Sonos/SonosDevice.h"

DeviceStateMachine::DeviceStateMachine(WifiManager& wifiManager, SonosDevice& sonosDevice) :
    m_currentState(State::Init),
    m_nextState(State::Init),
    m_deviceHandler(wifiManager, sonosDevice) {
}

DeviceStateMachine::~DeviceStateMachine(void) {
}

void DeviceStateMachine::resetStateMachine(void) {
    m_currentState = State::Init;
    m_nextState = State::Init;
}

void DeviceStateMachine::runStateMachine(void) {
    bool stateChanged = true;

    do {
        // reset state changed flag
        stateChanged = false;

        switch (m_currentState) {
            case State::Init:
                // only enter config mode when there is no stored network in DeviceSettings
                m_nextState = DeviceSettings::getWiFiConfig().isConfigured() ? State::Wifi_Connecting : State::Hotspot_Starting;
                break;
            case State::Hotspot_Starting:
                // TODO check is hotspot started
                conditionalStep(true, State::Hotspot_Idle);
                break;
            case State::Hotspot_Idle:
                // TODO
                break;
            case State::Wifi_Connecting:
                conditionalStep(m_deviceHandler.isWifiConnected(), State::Sonos_Connecting);
                break;
            case State::Sonos_Connecting:
                conditionalStep(m_deviceHandler.isSonosConnected(), State::Idle);
                conditionalStep(!m_deviceHandler.isSonosConnected(), State::Sonos_Retry);
                break;
            case State::Sonos_Retry:
                m_nextState = State::Sonos_Connecting;
                break;
            case State::Idle:
                // TODO
                break;
            default:
                Serial.println(F("State not implemented! Reset state machine."));
                resetStateMachine();
                break;
        }

        if (m_currentState != m_nextState) {
            Serial.print(F("State leave: ")); Serial.println(m_currentState);
            onLeaveState(m_currentState);

            // state change
            stateChanged = true;
            m_currentState = m_nextState;
            Serial.print(F("State enter: ")); Serial.println(m_currentState);
            onEnterState(m_currentState);
        }

        // run current state
        onRunState(m_currentState);

    } while (stateChanged);
}

void DeviceStateMachine::onEnterState(const State::Id state) {
    switch (state) {
        case State::Init:
            break;
        case State::Hotspot_Starting: {
                const DeviceSettings::DeviceParameters parameters = DeviceSettings::getDeviceParameters();
                m_deviceHandler.startHotspot(parameters);
            }
            break;
        case State::Hotspot_Idle:
            break;
        case State::Wifi_Connecting: {
            const DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
            m_deviceHandler.startWifi(config);
            }
            break;
        case State::Sonos_Connecting: {
            const DeviceSettings::SonosConfig config = DeviceSettings::getSonosConfig();
            m_deviceHandler.connectToSonos(config);
            }
            break;
        case State::Sonos_Retry:
            break;
        case State::Idle:
            break;
        default:
            break;
    }
}

void DeviceStateMachine::onRunState(const State::Id state) {
    switch (state) {
        case State::Init:
            break;
        case State::Hotspot_Starting:
            break;
        case State::Hotspot_Idle:
            break;
        case State::Wifi_Connecting:
            break;
        case State::Sonos_Connecting:
            break;
        case State::Sonos_Retry:
            break;
        case State::Idle:
            // TODO move as a const into SystemInitilizeDriver
            digitalWrite(2, LOW);
            break;
        default:
            break;
    }
}

void DeviceStateMachine::onLeaveState(const State::Id state) {
    switch (state) {
        case State::Init:
            break;
        case State::Hotspot_Starting:
            break;
        case State::Hotspot_Idle:
            break;
        case State::Wifi_Connecting:
            break;
        case State::Sonos_Connecting:
            break;
        case State::Sonos_Retry:
            break;
        case State::Idle:
            break;
        default:
            break;
    }
}

void DeviceStateMachine::conditionalStep(const bool isValid, const State::Id state) {
    if (isValid) {
        m_nextState = state;
    }
}