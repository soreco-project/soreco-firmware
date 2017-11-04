# soreco-firmware
Firmware for soreco Sonos Remote Control.
soreco is a hardware remote controller for Sonos wireless home sound system, which is currently in development.

[![Build Status](https://travis-ci.org/soreco-project/soreco-firmware.svg?branch=develop)](https://travis-ci.org/soreco-project/soreco-firmware)

## Usage
Open soreco-firmware.ino in Arduino and flash the NodeMCU 1.0 (ESP-12E Module).

The debug console is currently the only way to interact with the device. There are the following commands available to interact with the device:
* help (list all available commands)
* Config.Save (save configuration to persistent storage)
* Config.ClearAll (clear the persistent storage and initialize all values with 0)
* Config.FactoryDefaults (reset all configuration (except DeviceParameters) to factory defaults)
* Config.SerialNumber (get / set the serial number)
* Config.WiFi.SSID (get / set the WiFi SSID to connect to)
* Config.WiFi.Passphrase (get / set the WiFi passphrase for SSID)
* WiFi.Scan (scan and report all available networks)
* WiFi.Connect (connect to the configured network)
* WiFi.Status (print the WiFi status)

## Task List
[Link to the Trello](https://trello.com/b/eYdj9IFY/firmware)

## Special Thanks
Special Thanks to [Valentin](https://github.com/vmichalak) for his work on the [sonos-controller](https://github.com/vmichalak/sonos-controller) Android library, which served as a basis for the Arduino implementation.
