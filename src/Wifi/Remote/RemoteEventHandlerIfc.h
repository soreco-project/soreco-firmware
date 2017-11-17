#ifndef REMOTEEVENTHANDLERIFC_H
#define REMOTEEVENTHANDLERIFC_H

/**
 * An handler for all remote events.
 */
class RemoteEventHandlerIfc {
public:
    /**
     * Destructor.
     */
	virtual ~RemoteEventHandlerIfc() {
    }
    
    /**
     * Fired if new wifi configuration received. The configuration 
     * is stored in EEPROM.
     */
    virtual void onEventWifiConfigReceived(void) = 0;
    
};

#endif // REMOTEEVENTHANDLERIFC_H