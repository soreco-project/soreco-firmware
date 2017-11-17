#ifndef REMOTECOMMUNICATION_H
#define REMOTECOMMUNICATION_H

#include <ESP8266WiFi.h>

#include "Protocol/pb_decode.h"
#include "Protocol/pb_encode.h"
#include "Protocol/SorecoProtocol.pb.h"
#include "RemoteEventHandlerIfc.h"


/**
 * Handles communication with remote protocols which are implemented by google nano buffer. 
 */
class RemoteCommunication {
public:

    static const uint16_t PORT = 4000;
    static const uint16_t MAX_PACKAGE_SIZE = 1024;

    /**
    * Default constructor.
    */
    RemoteCommunication(RemoteEventHandlerIfc& remoteEventHandler);
    
    /**
    * Destructor.
    */
    ~RemoteCommunication(void);

    /**
     * Setup remote communication
     */
    void setup(void);

    /**
     * Handle incomings requests from TCP stack.
     */
    bool loop(void);

private:

    /**
    * Private copy constructor.
    */
    RemoteCommunication(const RemoteCommunication&);
    
     /**
     * Private assignment constructor.
     */
    RemoteCommunication& operator=(const RemoteCommunication&);

    _Protocol& processRequest(byte* pData);

    // handlers
    void handleWifiConfig(const WifiConfigMessage& msg);

    RemoteEventHandlerIfc& m_eventHandler;
    WiFiServer m_server;
};

#endif // REMOTECOMMUNICATION_H