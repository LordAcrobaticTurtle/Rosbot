#pragma once
/**
 * @brief File contains declaration of the micro controller CLI, TurtleShell
 * Must not use STL features, c style strings and processing only.
 */

#include <string.h>



enum CliCommandIndex {
    CLI_CLI,
    CLI_BEGIN,
    CLI_STANDBY,
    CLI_CALIBRATE,
    CLI_MOTOR,
    CLI_HELP,
    CLI_NUM_COMMANDS
};

const char cliCommands[CLI_NUM_COMMANDS+1][64] = {
    "cli",
    "Begin",
    "Standby",
    "Calibrate",
    "Motor",
    "Help",
    "None"
};

class TurtleShell {
    public:
        TurtleShell() : m_isShellActive(false) {};
        ~TurtleShell() {};

        /**
         * @brief Parses buffer and returns the command type
         * @param buffer Assumes buffer contains the command and nothing else. Should be a null terminated string
         * @param bufferLength 
         * @return CliCommandIndex 
         */
        CliCommandIndex parseCommand(const char *buffer, unsigned int bufferLength) {
            
            // Test if buffer is equal to cli
            if (strcmp(buffer, cliCommands[CLI_CLI]) == 0) { toggleShell(); }

            if (!m_isShellActive) {
                return CLI_NUM_COMMANDS;
            }

            for (int command = 0; command < CliCommandIndex::CLI_NUM_COMMANDS; command++) { 
                if (strcmp(buffer, cliCommands[command]) == 0) {
                    return (CliCommandIndex) command;
                }
            }

            return CLI_NUM_COMMANDS;
        }
    
        void toggleShell() {
            m_isShellActive = !m_isShellActive;
        }

    protected:
        bool m_isShellActive;


};
