#include "DebugConsole.h"

#include <Arduino.h>

#include "SerialCommands.h"
#include "DeviceSettings.h"

SerialCommands serialCommands;

void cmdHelp(void) {
    serialCommands.listCommands();
}

void cmdSerialNumberGet(void) {
    DeviceSettings::DeviceParameters params = DeviceSettings::getDeviceParameters();
    Serial.println(params.deviceSerialNumber);
}

void cmdSerialNumberSet(void) {
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

void cmdEepromSave(void) {
    DeviceSettings::save();
}

DebugConsole::DebugConsole(void) {
}

DebugConsole::~DebugConsole(void) {
}

void DebugConsole::setup(void) {
    serialCommands.addCommand("help", cmdHelp);
    serialCommands.addCommand("Config.SerialNumber.Get", cmdSerialNumberGet);
    serialCommands.addCommand("Config.SerialNumber.Set", cmdSerialNumberSet);
    serialCommands.addCommand("Config.Save", cmdEepromSave);
}

void DebugConsole::loop(void) {
    serialCommands.loop();
}