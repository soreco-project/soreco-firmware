#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include <stdint.h>
#include "../WifiManager/WifiManager.h"
#include "../Sonos/SonosDevice.h"
#include "../DeviceSettings/DeviceSettings.h"

/**
 * Handler for state machine events
 */
class DeviceHandler {
public:
    /**
    * Default constructor.
    */
    DeviceHandler(WifiManager& wifiManager, SonosDevice& sonosDevice);

    /**
    * Destructor.
    */
    ~DeviceHandler(void);

    // Wifi events
    bool isWifiConfigured(void) const;
    bool isWifiConnected(void) const;
    void startWifi(void);
    void startHotspot(void);

    // Sonos events
    bool isSonosConfigured(void) const;
    bool isSonosConnected(void) const;
    void connectToSonos(void);

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
    DeviceHandler(const DeviceHandler&);

    /**
    * Private assignment constructor.
    */
    DeviceHandler& operator=(const DeviceHandler&);

    void setSonosCoordinator(SonosDevice& sonosCoordinator);

    WifiManager& m_wiFiManager;
    SonosDevice& m_sonosCoordinator;
    bool m_sonosConnected;
};

#endif //DEVICEHANDLER_H