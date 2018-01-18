#ifndef SERIALCOMMANDS_H
#define SERIALCOMMANDS_H

#include <stdint.h>

/**
 * SerialCommands provides a simple command line interface with callbacks for each command.
 * Once the line terminator is received, the buffered characters are processed and the registered callback is invoked.
 */
class SerialCommands {
public:

    /**
    * Default constructor.
    */
    SerialCommands(void);

    /**
     * Destructor.
     */
    ~SerialCommands(void);

    /**
    * One time configuration.
    */
    void setup(void);

    /**
    * Process console input.
    */
    void loop(void);

    /**
     * Register a function callback to the given command
     */
    void addCommand(const char* cmd, void(*function)());

    /**
     * Get the next argument of a command
     */
    char* getArgument(void);

    /**
     * List all available commands on the Serial console.
     */
    void listCommands(void) const;

private:

    /**
    * Private copy constructor.
    */
    SerialCommands(const SerialCommands&);

    /**
    * Private assignment operator.
    */
    SerialCommands& operator=(const SerialCommands&);

    void clearBuffer(void);

    static const uint8_t MAX_COMMAND_LENGTH = 128;
    static const uint8_t MAX_COMMANDS = 32;
    const char* COMMAND_DELIMITER = " ";
    const char COMMAND_TERMINATOR = '\n';

    // Data structure to hold Command/Handler function key-value pairs
    struct SerialCommandsCallback {
        char command[MAX_COMMAND_LENGTH];
        void (*function)(void);
    };

    // buffer for characters until terminator received
    char m_buffer[MAX_COMMAND_LENGTH];
    char* m_argumentSavePtr;
    uint8_t m_bufferPos;
    uint8_t m_numberOfCommands;
    SerialCommandsCallback m_commands[MAX_COMMANDS];
};

#endif //SERIALCOMMANDS_H
