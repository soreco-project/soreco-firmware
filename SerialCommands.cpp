#include "SerialCommands.h"

#include <Arduino.h>

SerialCommands::SerialCommands(void) :
	m_argumentSavePtr(NULL),
	m_bufferPos(0),
	m_numberOfCommands(0)
{
    clearBuffer(); 
}

void SerialCommands::setup(void) {
}

void SerialCommands::loop(void) {
    while (Serial.available() > 0) {
        char ch = Serial.read();	
        if (ch == COMMAND_TERMINATOR) {
			char* token = strtok_r(m_buffer, COMMAND_DELIMITER, &m_argumentSavePtr);
			bool commandFound = false;
			if (token != NULL) {
				for (int i = 0; i < m_numberOfCommands; i++) {
					// Compare the found command against the list of known commands for a match
					String userCommand = token;
					if(userCommand.equalsIgnoreCase(m_commands[i].command)) {
						// Execute the stored handler function for the command
						(*m_commands[i].function)(); 
						commandFound = true;
						break; 
					}
				}
			}
			if (!commandFound) {
				listCommands();
			}
			clearBuffer();
		}
		else {
			// store and wrap around if full
			m_buffer[m_bufferPos] = ch;
			m_bufferPos = (m_bufferPos + 1) % sizeof(m_buffer);
			// ensure null termination even in case of wrap around
			m_buffer[m_bufferPos] = '\0';
		}
    }
}

void SerialCommands::clearBuffer(void) {
    for (int i = 0; i < sizeof(m_buffer); i++)  {
        m_buffer[i] = '\0';
    }
	m_bufferPos = 0;
	m_argumentSavePtr = NULL;
}

char* SerialCommands::getArgument(void)  {
    return strtok_r(NULL, COMMAND_DELIMITER, &m_argumentSavePtr);
}

void SerialCommands::addCommand(const char *cmd, void (*function)(void)) {
    if (m_numberOfCommands < MAX_COMMANDS) {
        strncpy(m_commands[m_numberOfCommands].command, cmd, MAX_COMMAND_LENGTH); 
        m_commands[m_numberOfCommands].function = function; 
        m_numberOfCommands++;
	}
	else {
        Serial.println("ERROR: SerialCommands - reached commands limit"); 
    }
}

void SerialCommands::listCommands(void) {
	Serial.println();
	for(int i = 0; i < m_numberOfCommands; i++) {
		Serial.println(m_commands[i].command);
	}
}
