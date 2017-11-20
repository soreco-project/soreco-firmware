#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <stdint.h>
#include <vector>
#include <ESP8266WiFi.h>

/**
 * Wifi manager to handle all interaction with the wifi module.
 */
class WifiManager {
public:

    struct WiFiNetwork {
        std::string ssid;
        // signal strength in dBm
        int32_t signalStrength;
        wl_enc_type encryptionType;
    };

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
     * Set system into hotspot (access point) mode so an external device can connect for configuration.
     * Note: hotspot name = soreco-xyz (xyz = deviceSerialNumber)
     */
    void startConfigMode(const uint32_t deviceSerialNumber);

    /**
     * Set system into client mode so it can connect to the given WiFi.
     */
    void startClientMode(const char* ssid, const char* passphrase);

    /**
     * Scans and returns the available WiFi networks.
     */
    std::vector<WiFiNetwork> scanForNetworks(void);

    /**
     * Returns true if wifi module is connected to an access point
     */
    bool isWifiConnected(void) const;

private:
    /**
     * Private copy constructor.
     */
    WifiManager(const WifiManager&);

    /**
     * Private assignment operator.
     */
    WifiManager& operator=(const WifiManager&);
};

#endif //WIFIMANAGER_H
