#include "DeviceSettings.h"

#include <Arduino.h>
#include <assert.h>
#include <EEPROM.h>

/**
 * ESP8622 EEPROM offers up to 4096 bytes (one flash block/sector).
 * When you call EEPROM.begin(), that block is read and put into memory. 
 * From then on unless you call commit() or end(), you are always working with that memory block and not the actual content of the flash block.
 * When you call end() or commit(), the memory block is checked for changes and if it has any, the flash block is erased and the memory block is dumped to the flash to save the data for next time.
 * Flash memory has a particular number of erase/write cycles on its bits (around 10'000), so commit() calls need to be optimized to prevent wearing the flash too soon.
 */
DeviceSettings::DeviceSettings(void) {    
}

void DeviceSettings::load(void) {
    assert(CURRENT_EEPROM_LAYOUT_SIZE_BYTES <= 4096);
    Serial.print(F("DeviceSettings: Current EEPROM layout size = ")); Serial.println(CURRENT_EEPROM_LAYOUT_SIZE_BYTES);
    // ESP8622 needs EEPROM.begin(), so it copies the flash block into a memory area
    EEPROM.begin(CURRENT_EEPROM_LAYOUT_SIZE_BYTES);

    // in case of a downgrade from a unknown EEPROM layout version, we simply restore the factory factory defaults
    uint8_t layoutVersion = EEPROM.read(0);
    if (layoutVersion != CURRENT_EEPROM_LAYOUT_VERSION) {
        Serial.println(F("Warning - unknown EEPROM layout version! Restoring factory defaults"));
        factoryDefaults();
    }
}

void DeviceSettings::save(void) {
    EEPROM.commit();
}

void DeviceSettings::clearAll(void) {
    for (int i = 0; i < 4096; i++) {
        EEPROM.write(i, 0);
    }
    save();
}

void DeviceSettings::factoryDefaults(void) {
    // preserve serial number
    DeviceParameters parameters = getDeviceParameters();
    clearAll();
    EEPROM.write(0, CURRENT_EEPROM_LAYOUT_VERSION);
    setDeviceParameters(parameters);

    // default device config
    DeviceConfig deviceConfig;
    deviceConfig.configFlags.fields.maxVolumeLimitActive = false;
    deviceConfig.configFlags.fields.stayAlwaysOnActive = false;
    deviceConfig.maxVolume = 50;
    deviceConfig.deepSleepTimeout = 30;
    setDeviceConfig(deviceConfig);

    save();
}

DeviceSettings::DeviceParameters DeviceSettings::getDeviceParameters(void) {
    DeviceParameters eepromValues;
    size_t offset = offsetof(struct EepromLayoutV1, deviceParameters);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setDeviceParameters(const DeviceParameters& param) {
    size_t offset = offsetof(struct EepromLayoutV1, deviceParameters);
    EEPROM.put(offset, param);
}

DeviceSettings::DeviceConfig DeviceSettings::getDeviceConfig(void) {
    DeviceConfig eepromValues;
    size_t offset = offsetof(struct EepromLayoutV1, deviceConfig);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setDeviceConfig(const DeviceConfig& param) {
    size_t offset = offsetof(struct EepromLayoutV1, deviceConfig);
    EEPROM.put(offset, param);
}

DeviceSettings::SonosConfig DeviceSettings::getSonosConfig(void) {
    SonosConfig eepromValues;
    size_t offset = offsetof(struct EepromLayoutV1, sonosConfig);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setSonosConfig(const SonosConfig& param) {
    size_t offset = offsetof(struct EepromLayoutV1, sonosConfig);
    EEPROM.put(offset, param);
}

DeviceSettings::WiFiConfig DeviceSettings::getWiFiConfig(void) {
    WiFiConfig eepromValues;
    size_t offset = offsetof(struct EepromLayoutV1, wifiConfig);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setWiFiConfig(const WiFiConfig& param) {
    size_t offset = offsetof(struct EepromLayoutV1, wifiConfig);
    EEPROM.put(offset, param);
}

DeviceSettings::PresetConfig DeviceSettings::getPresetConfig(uint8_t index) {
    PresetConfig eepromValues;
    size_t offset = getPresetConfigOffset(index);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setPresetConfig(uint8_t index, const PresetConfig& param) {
    size_t offset = getPresetConfigOffset(index);
    EEPROM.put(offset, param);
}

size_t DeviceSettings::getPresetConfigOffset(uint8_t index) {
    size_t offset = 0;
    switch (index) {
        case 0:
            offset = offsetof(struct EepromLayoutV1, preset1);
            break;
        case 1:
            offset = offsetof(struct EepromLayoutV1, preset2);
            break;
        case 2:
            offset = offsetof(struct EepromLayoutV1, preset3);
            break;
        case 3:
            offset = offsetof(struct EepromLayoutV1, preset4);
            break;
        default:
            assert(false);
            break;
    }
    return offset;
}