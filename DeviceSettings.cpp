#include "DeviceSettings.h"

#include <EEPROM.h>

DeviceSettings::DeviceSettings() {
    EEPROM.begin(CURRENT_EEPROM_LAYOUT_SIZE_BYTES);
    loadEeprom();
}

void DeviceSettings::loadEeprom() {
    loadEepromLayoutV1();
}

void DeviceSettings::loadEepromLayoutV1() {

}
