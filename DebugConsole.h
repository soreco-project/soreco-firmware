#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

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
    * Register console commands.
    */
    void setup(void);

    /**
    * Process console input.
    */
    void loop(void);
};

#endif //DEBUGCONSOLE_H
