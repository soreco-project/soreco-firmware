#include "DeviceStateMachine.h"

#include <Arduino.h>
#include "../DeviceSettings/DeviceSettings.h"
#include "../WifiManager/WifiManager.h"
#include "../Sonos/SonosDevice.h"

DeviceStateMachine::DeviceStateMachine(WifiManager& wifiManager, SonosDevice& sonosDevice) : 
	m_deviceHandler(wifiManager, sonosDevice) {
}

DeviceStateMachine::~DeviceStateMachine(void) {
}

void DeviceStateMachine::resetStateMachine(void) {
	m_currentState = StateMachineState_Init;
	m_nextState = StateMachineState_Init;
}

void DeviceStateMachine::runStateMachine(void) {
	bool stateChanged = true;
	
	do {
		// reset state changed flag
		stateChanged = false;

		switch (m_currentState) {
			case StateMachineState_Init: 
	        	// only enter config mode when there is no stored network in DeviceSettings			
				m_nextState = DeviceSettings::getWiFiConfig().isConfigured() ? StateMachineState_Wifi_Connecting : StateMachineState_Hotspot_Starting;
				break;
			case StateMachineState_Hotspot_Starting:
				// TODO check is hotspot started
				conditionalStep(true, StateMachineState_Hotspot_Idle);
				break;
			case StateMachineState_Hotspot_Idle:
				// TODO
				break;
			case StateMachineState_Wifi_Connecting:
				// TODO if wifi connected go to STM_Sonos_Zone_Connecting	
				conditionalStep(m_deviceHandler.isWifiConnected(), StateMachineState_Idle);
				break;
			case StateMachineState_Idle:
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

void DeviceStateMachine::onEnterState(const StateMachineState state) {
	switch (state) {
		case StateMachineState_Init:
			break;
		case StateMachineState_Wifi_Connecting: {
				const DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();
				m_deviceHandler.startWifi(config);			
			}
			break;
		case StateMachineState_Hotspot_Starting: {
				const DeviceSettings::DeviceParameters parameters = DeviceSettings::getDeviceParameters();
				m_deviceHandler.startHotspot(parameters);
			}
			break;
		case StateMachineState_Hotspot_Idle:
			break;
		case StateMachineState_Idle:
			break;
		default:
			break;
	}
}

void DeviceStateMachine::onRunState(const StateMachineState state) {
	switch (state) {
		case StateMachineState_Init:
			break;
		case StateMachineState_Wifi_Connecting:
			break;
		case StateMachineState_Hotspot_Starting:
			break;
		case StateMachineState_Hotspot_Idle:
			break;
		case StateMachineState_Idle:
			break;
		default:
			break;
	}
}

void DeviceStateMachine::onLeaveState(const StateMachineState state) {
	switch (state) {
		case StateMachineState_Init:
			break;
		case StateMachineState_Wifi_Connecting:
			break;
		case StateMachineState_Hotspot_Starting:
			break;
		case StateMachineState_Hotspot_Idle:
			break;
		case StateMachineState_Idle:
			break;
		default:
			break;
	}
}

void DeviceStateMachine::conditionalStep(const bool isValid, const StateMachineState state) {
	if (isValid) {
		m_nextState = state;
	}
}