#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include <stdint.h>
#include "../Wifi/WifiManager.h"
#include "../Wifi/Remote/RemoteEventHandlerIfc.h"
#include "../Sonos/SonosDevice.h"
#include "../DeviceSettings/DeviceSettings.h"

/**
 * Handler for state machine events
 */
class DeviceHandler : public RemoteEventHandlerIfc {
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

    // input event handlers TODO extract to InputEventIfc
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
    
    // @see RemoteCommunicationIfc
    virtual void onEventWifiConfigReceived(void);
    
    bool isWifiConfigChanged(void);
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

    // components
    WifiManager& m_wiFiManager;
    SonosDevice& m_sonosCoordinator;
    
    // flags
    bool m_sonosConnected;
    bool m_wifiConfigEventPending;
};

#endif //DEVICEHANDLER_H