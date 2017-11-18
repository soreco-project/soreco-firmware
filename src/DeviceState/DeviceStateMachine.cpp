#include "DeviceStateMachine.h"

#include <Arduino.h>
#include "../WifiManager/WifiManager.h"
#include "../Sonos/SonosDevice.h"

DeviceStateMachine::DeviceStateMachine(WifiManager& wifiManager, SonosDevice& sonosDevice) :
    m_currentState(State::Init),
    m_nextState(State::Init),
    m_deviceHandler(wifiManager, sonosDevice),
    m_sonosRetryTimestampMs(0) {
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
                // only enter config mode when there is no stored network
                m_nextState = m_deviceHandler.isWifiConfigured() ? State::Wifi_Connecting : State::Hotspot_Starting;
                break;
            case State::Hotspot_Starting:
                // TODO check is hotspot started
                conditionalStep(true, State::Hotspot_Idle);
                break;
            case State::Hotspot_Idle:
                // TODO
                break;
            case State::Wifi_Connecting:
                if (m_deviceHandler.isWifiConnected()) {
                    m_nextState = m_deviceHandler.isSonosConfigured() ? State::Sonos_Connecting : State::Idle;
                }
                break;
            case State::Sonos_Connecting:
                m_nextState = m_deviceHandler.isSonosConnected() ? State::Idle : State::Sonos_Retry;
                break;
            case State::Sonos_Retry:
                if ((millis() - m_sonosRetryTimestampMs) >= SONOS_RETRY_DELAY_MS) {
                    // retry timeout expired, let's try again
                    Serial.println(F("Sonos retry timeout expired - attempt to reconnect"));
                    m_nextState = m_deviceHandler.isWifiConnected() ? State::Sonos_Connecting : State::Wifi_Connecting;
                }
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
        case State::Hotspot_Starting:
            m_deviceHandler.startHotspot();
            break;
        case State::Hotspot_Idle:
            break;
        case State::Wifi_Connecting:
            m_deviceHandler.startWifi();
            break;
        case State::Sonos_Connecting:
            m_deviceHandler.connectToSonos();
            break;
        case State::Sonos_Retry:
            m_sonosRetryTimestampMs = millis();
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