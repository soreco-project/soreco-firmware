#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <stdint.h>
#include <vector>
#include "ESP8266WiFi.h"

/**
 * Wifi manager to handle all interaction with the wifi module.
 */
class WifiManager {
public:

    struct WiFiNetwork {
        String ssid;
        // signal strength in dBm
        int32_t signalStrength;
        wl_enc_type encryptionType;
    };

    static const uint16_t PORT = 4000;
    const char* CONFIG_SSID_PREFIX = "soreco-";

    /**
     * Default constructor.
     */
    WifiManager(void);

    /**
     * Deconstructor.
     */
    ~WifiManager(void);

    /**
     * Setup the current WiFi manager
     */
    void setup(void);
    
    /**
     * handle TCP. TODO do it with irq!
     */
    void loop(void);

    /**
     * Set system into hotspot (access point) mode so an external device can connect for configuration. 
     */
    void startConfigMode(void);

    /**
     * Set system into client mode so it can connect to the given WiFi.
     */
    void startClientMode(char* ssid, char* passphrase);

    /**
     * Scans and returns the available WiFi networks.
     */
    std::vector<WiFiNetwork> scanForNetworks(void);

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
