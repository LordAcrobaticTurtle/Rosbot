#pragma once
/**
 * @brief File contains declaration of the micro controller CLI, TurtleShell
 * Must not use STL features, c style strings and processing only.
 */

#include <string.h>
#include "data-structures/circular_queue.h"

#define CLI_MAX_NUM_ARGUMENTS 5
#define CLI_MAX_ARGUMENT_LENGTH 16

enum CliCommandIndex {
    CLI_CLI,
    CLI_BEGIN,
    CLI_STANDBY,
    CLI_CALIBRATE,
    CLI_RESET_IMU,
    CLI_MOTOR,
    CLI_HELP,
    CLI_NUM_COMMANDS
};

const char cliCommands[CLI_NUM_COMMANDS+1][64] = {
    "cli",
    "Begin",
    "Standby",
    "Calibrate",
    "Reset-IMU", 
    "Motor",
    "Help",
    "None"
};

/**
 * @brief Can only handle 5 arguments in a single command as per the macro CLI_MAX_NUMBER_ARGUMENTS at time of writing.
 * Shell starts active
 * 
 */
class TurtleShell {
    public:
        TurtleShell() : m_isShellActive(true) {};
        ~TurtleShell() {};

        /**
         * @brief Look for a command in a circular queue. If a command is successfully found the queue must be wiped
         * @param queue 
         * @return CliCommandIndex 
         */
        CliCommandIndex searchForCommand(CircularQueue &queue, int &argc, char argv[][CLI_MAX_ARGUMENT_LENGTH]);

    protected:

        /**
         * @brief Finds first instance of ' ' character and copies the data from the start up to
         * that delimiter into buffer. The final character is replaced with a null terminator.
         * @param queue 
         * @param buffer 
         * @return CliCommandIndex 
         */
        int findCommandInPacket(CircularQueue &queue, byte* buffer);
        
        /**
         * @brief Parses buffer and returns the command type
         * @param buffer Assumes buffer contains the command and nothing else. Should be a null terminated string
         * @param bufferLength 
         * @return CliCommandIndex 
         */
        CliCommandIndex parseCommand(const char *buffer, unsigned int bufferLength);

        /**
         * @brief Checks the queue for a newline character
         * @return true 
         * @return false 
         */
        bool isNewlinePresent(CircularQueue &queue);

        void toggleShell() {
            m_isShellActive = !m_isShellActive;
        }

    protected:
        bool m_isShellActive;


};
