#include "DebugConsole.h"

#include <Arduino.h>

#include "SerialCommands.h"
#include "DeviceSettings.h"

SerialCommands serialCommands;

void cmdHelp(void) {
    serialCommands.listCommands();
}

void cmdConfigSerialNumberGet(void) {
    DeviceSettings::DeviceParameters params = DeviceSettings::getDeviceParameters();
    Serial.println(params.deviceSerialNumber);
}

void cmdConfigSerialNumberSet(void) {
    DeviceSettings::DeviceParameters params;
    char* argument = serialCommands.getArgument();
    if (argument != NULL) {
        params.deviceSerialNumber = atoi(argument);
        DeviceSettings::setDeviceParameters(params);
    }
    else {
        Serial.println("Error - expecting argument 'serialNumber' as number!");
    }
}

void cmdConfigSave(void) {
    DeviceSettings::save();
}

void cmdConfigClearAll(void) {
    DeviceSettings::clearAll();
}

void cmdConfigFactoryDefaults(void) {
    DeviceSettings::factoryDefaults();
}

DebugConsole::DebugConsole(void) {
}

DebugConsole::~DebugConsole(void) {
}

void DebugConsole::setup(void) {
    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.SerialNumber.Get", cmdConfigSerialNumberGet);
    serialCommands.addCommand("Config.SerialNumber.Set", cmdConfigSerialNumberSet);
    serialCommands.addCommand("Config.Save", cmdConfigSave);
    serialCommands.addCommand("Config.ClearAll", cmdConfigClearAll);
    serialCommands.addCommand("Config.FactoryDefaults", cmdConfigFactoryDefaults);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}