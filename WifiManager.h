#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <stdint.h>
#include "ESP8266WiFi.h"
#include <WiFiClient.h> 

/**
 * Wifi manager to handle all interaction with the wifi module.
 */
class WifiManager {
public:    
    static const uint16_t PORT = 4000;
    static const bool SSID_HIDDEN = false;
    const char* SSID = "ESPap";
    const char* PASSWORD = "12345678";

    /**
     * Default constructor.
     */
    WifiManager(void);    

    /**
     * Deconstructor.
     */
    ~WifiManager(void);    

    /**
     * set ESP8266 into pairing mode so an external can join to access point. 
     */
    void startServiceMode();

    /**
     * Scan and print all networks that available.
     */
    void scanForNetworks(void);

    /**
     * handle TCP. TODO do it with irq!
     */
    void loop(void);

    /**
     * Setup the current WiFi manager
     */
    void setup(void);
private:    
    /**
     * Private copy constructor.
     */
    WifiManager(const WifiManager&);

    /**
     * Private assignment constructor.
     */
    WifiManager& operator=(const WifiManager&);

    WiFiServer m_server;
};

#endif //WIFIMANAGER_H
