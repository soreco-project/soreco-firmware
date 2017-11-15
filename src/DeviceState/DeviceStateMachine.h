#ifndef DEVICESTATEMACHINE_H
#define DEVICESTATEMACHINE_H

#include <stdint.h>
#include "DeviceHandler.h"

/**
 * Command line interface for development to interact with the device.
 */
class DeviceStateMachine {
public:

    /**
     * States provided by this state machine
     */
    enum StateMachineState {
        StateMachineState_Init,
        StateMachineState_Hotspot_Starting,
        StateMachineState_Hotspot_Idle,
        StateMachineState_Wifi_Connecting,
        StateMachineState_Idle,
    };

    /**
    * Default constructor.
    */
    DeviceStateMachine(WifiManager& wifiManager, SonosDevice& sonosDevice);

    /**
    * Destructor.
    */
    ~DeviceStateMachine(void);

    /**
     * Runs the state machine. Call this method for running the state machine.
     */
    void runStateMachine(void);

    /**
     * Rests the state machine.
     */
    void resetStateMachine(void);
private:

    /**
    * Private copy constructor.
    */
    DeviceStateMachine(const DeviceStateMachine&);

    /**
    * Private assignment constructor.
    */
    DeviceStateMachine& operator=(const DeviceStateMachine&);

    void onEnterState(const StateMachineState state);
    void onRunState(const StateMachineState state);
    void onLeaveState(const StateMachineState state);

    /**
     * Switch to the given state if conditional valid
     */
    void conditionalStep(const bool isValid, const StateMachineState state);

    StateMachineState m_currentState;
    StateMachineState m_nextState;
    DeviceHandler m_deviceHandler;
};

#endif //DEVICESTATEMACHINE_H