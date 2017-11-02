#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <stdint.h>
#include <stddef.h>

/**
 * Persistent storage of device settings.
 * Built for future extensibility across multiple software updates with layout versioning system. 
 * Note: save() has to be called in order to persist the data
 */
class DeviceSettings {
public:

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
        // name of soreco
        char deviceName[32+1] = "";
        // upper threshold for volume
        uint8_t maxVolume;
        // timeout in minutes when device should go to deep sleep after last user interaction
        uint16_t deepSleepTimeout;
    };

    struct SonosConfig {
        // sonos device to control
        char sonosDeviceName[32+1] = "";
    };
    
    struct WiFiConfig {
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
        char passphrase[63+1] = "";

        struct IpConfig {
            uint32_t address;
            uint32_t subnetMask;
            uint32_t dnsServer;
            uint32_t gateway;
        };
        IpConfig fixedIpConfig;
    };

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

    /**
     * Load settings from persistent storage and keep data in memory.
     */
    static void load(void);

    /**
     * Save settings from memory to persistent storage.
     */
    static void save(void);

    static DeviceParameters getDeviceParameters(void);
    static void setDeviceParameters(const DeviceParameters& param);

    static DeviceConfig getDeviceConfig(void);
    static void setDeviceConfig(const DeviceConfig& param);

    static SonosConfig getSonosConfig(void);
    static void setSonosConfig(const SonosConfig& param);

    static WiFiConfig getWiFiConfig(void);
    static void setWiFiConfig(const WiFiConfig& param);

    static PresetConfig getPresetConfig(uint8_t index);
    static void setPresetConfig(uint8_t index, const PresetConfig& param);

private:
    /**
    * Default constructor.
    */
    DeviceSettings(void);    

    /**
     * Private copy constructor.
     */
    DeviceSettings(const DeviceSettings&);

    /**
     * Private assignment constructor.
     */
    DeviceSettings& operator=(const DeviceSettings&);

    /**
     * Calculates the offset in bytes for the given preset configuration within the EEPROM layout structure.
     * @param index - index of preset (0..3)
     */
    static size_t getPresetConfigOffset(uint8_t index);

    /**
     * ESP8266 needs 4 byte alignment since it emulates EEPROM in flash.
     * Note: sizeof() and offsetof(type, member) will calculate offsets taking alignment rules into account.
     */
    struct EepromLayoutV1 {
        // maximum EEPROM size is limited to 4096 bytes!
        uint8_t layoutVersion = EEPROM_LAYOUT_VERSION_V1;
        DeviceParameters deviceParameters;
        DeviceConfig deviceConfig;
        SonosConfig sonosConfig;
        WiFiConfig wifiConfig;
        PresetConfig preset1;
        PresetConfig preset2;
        PresetConfig preset3;
        PresetConfig preset4;
    }; __attribute__ ((aligned (4)));

    // constants
    // in order to extend the layout in future, we need to have a versioned layout of the EEPROM structure
    static const uint16_t EEPROM_LAYOUT_VERSION_V1 = 1;
    static const uint16_t CURRENT_EEPROM_LAYOUT_VERSION = EEPROM_LAYOUT_VERSION_V1;
    static const uint16_t CURRENT_EEPROM_LAYOUT_SIZE_BYTES = sizeof(EepromLayoutV1);

    // instance members
};

#endif //DEVICESETTINGS_H