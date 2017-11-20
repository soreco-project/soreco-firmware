#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include "../Wifi/WifiManager.h"
#include "../Sonos/SonosDevice.h"

/**
 * Command line interface for development to interact with the device.
 */
class DebugConsole {
public:

    /**
    * Default constructor.
    */
    DebugConsole(void);

    /**
    * Destructor.
    */
    ~DebugConsole(void);

    /**
    * Register console commands.
    */
    void setup(WifiManager& wifiManager, SonosDevice& sonosDevice);

    /**
    * Process console input.
    */
    void loop(void);

private:

    /**
    * Private copy constructor.
    */
    DebugConsole(const DebugConsole&);

    /**
    * Private assignment operator.
    */
    DebugConsole& operator=(const DebugConsole&);
};

#endif //DEBUGCONSOLE_H
