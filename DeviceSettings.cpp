#include "DeviceSettings.h"

#include <Arduino.h>
#include <assert.h>
#include <EEPROM.h>
#include <stddef.h>

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
    Serial.print("DeviceSettings: Loading EEPROM layout with size = "); Serial.println(CURRENT_EEPROM_LAYOUT_SIZE_BYTES);

    // ESP8622 needs EEPROM.begin(), so it copies the flash block into a memory area
    EEPROM.begin(CURRENT_EEPROM_LAYOUT_SIZE_BYTES);
}

void DeviceSettings::save(void) {
    EEPROM.commit();
}

DeviceSettings::WiFiSettings DeviceSettings::getWiFiSettings(void) {
    WiFiSettings eepromValues;
    size_t offset = offsetof(struct EepromLayoutV1, wifiSettings);
    EEPROM.get(offset, eepromValues);
    return eepromValues;
}

void DeviceSettings::setWiFiSettings(const WiFiSettings& settings) {
    size_t offset = offsetof(struct EepromLayoutV1, wifiSettings);
    EEPROM.put(offset, settings);
}