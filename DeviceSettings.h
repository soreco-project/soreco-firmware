/*************************************************** 
 Persistent storage of device settings.
 Built for future extensibility across multiple software updates with layout versioning system. 
 ****************************************************/

#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <stdint.h>

class DeviceSettings {
public:
    /**
    * Default constructor.
    */
    DeviceSettings(void);

    /**
     * Erase persistent storage and restore factory defaults.
     */
    void resetFactoryDefaults(void);

private:
    /**
     * Private copy constructor.
     */
    DeviceSettings(const DeviceSettings&);

    /**
     * Private assignment constructor.
     */
    DeviceSettings& operator=(const DeviceSettings&);

    void loadEeprom(void);
    void storeEeprom(void);

    void loadEepromLayoutV1(void);

    void loadWiFiCredentials(void);
    void storeWiFiCredentials(void);

    // maximum EEPROM size is limited to 4096 bytes!
    struct EepromLayoutV1 {
        uint8_t layoutVersion;
        
        // cannot be configured by the user and is configured in production
        struct DeviceParameters {
            uint32_t deviceSerialNumber;
        };

        struct DeviceConfig {
            union ConfigFlags {
                struct {
                    bool maxVolumeLimitActive : 1;
                    bool stayAlwaysOnActive : 1;
                    // 30 flags remaining for future use
                };
                uint32_t flags;
            };
            char deviceName[32+1] = "";
            // upper threshold the volume can be set (e.g. as a child lock)
            uint8_t maxVolume;
            // timeout in minutes when device should go to deep sleep after last user interaction
            uint16_t deepSleepTimeout;
        };
        
        struct WiFiSettings {
            union ConfigFlags {
                struct {
                    bool dhcpActive : 1;
                    // 32 flags remaining for future use
                };
                uint32_t flags;
            };
            // max SSID length is 32 characters, reserve 1 additional byte for string null termination
            char ssid[32+1] = "";
            // max passphrase length for WPA-PSK is 63 characters
            char passphrases[63+1] = "";

            struct IpConfig {
                uint32_t address;
                uint32_t subnetMask;
                uint32_t dnsServer;
                uint32_t gateway;
            };
            IpConfig fixedIpConfig;
        };

        // Sonos device to control
        char sonosDeviceName[32+1] = "";

        struct PresetConfig {
            struct Action {
                enum ActionType {
                    PLAY_URI = 1,
                    VOLUME = 2,
                    GROUP_ALL_PLAYERS = 3,
                    TURN_OFF_TIMER = 4
                };
                uint8_t actionIdentifier;
                // TODO: better way to be flexible for the future?
                uint8_t actionParameters[32];    
            };
            
            char mediaUri[255+1] = "";
            uint8_t numberOfActionsActive;
            Action actions[4];
        };
        PresetConfig preset1;
        PresetConfig preset2;
        PresetConfig preset3;
        PresetConfig preset4;
    };

    // constants
    // in order to extend the layout in future, we need to have a versioned layout of the EEPROM structure
    static const uint16_t EEPROM_LAYOUT_VERSION_V1 = 1;
    static const uint16_t CURRENT_EEPROM_LAYOUT_VERSION = EEPROM_LAYOUT_VERSION_V1;
    static const uint16_t CURRENT_EEPROM_LAYOUT_SIZE_BYTES = 2048;

    // instance members
};

#endif //DEVICESETTINGS_H