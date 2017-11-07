#ifndef DEVICESTATEMACHINE_H
#define DEVICESTATEMACHINE_H

#include <stdint.h>

/**
 * Command line interface for development to interact with the device.
 */
class DeviceStateMachine {
public:

    /**
    * Default constructor.
    */
    DeviceStateMachine(void);

    /**
    * Destructor.
    */
    ~DeviceStateMachine(void);

    // input event handlers
    void onEventVolumeUp(uint16_t volumeStepCount);
    void onEventVolumeDown(uint16_t volumeStepCount);
    void onEventPlayPause(void);
    void onEventNext(void);
    void onEventPrevious(void);
    void onEventPreset1(void);
    void onEventPreset2(void);
    void onEventPreset3(void);
    void onEventPreset4(void);
    void onEventConfigMode(void);
    void onEventRestart(void);

private:

    /**
    * Private copy constructor.
    */
    DeviceStateMachine(const DeviceStateMachine&);
   
    /**
    * Private assignment constructor.
    */
    DeviceStateMachine& operator=(const DeviceStateMachine&);

};

#endif //DEVICESTATEMACHINE_H