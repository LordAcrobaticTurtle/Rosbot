#include "comms/cli/turtle_shell.h"
#include <unistd.h>

CliCommandIndex TurtleShell::searchForCommand(CircularQueue &queue, int &argc, char argv[][CLI_MAX_ARGUMENT_LENGTH]) {
    
    // Search for a newline
    // If none exists, return early. 
    // If one does exist, look for parse for commands and arguments
    if (isNewlinePresent(queue) == -1) {
        return CLI_NUM_COMMANDS;
    }
    // Replace the newline with a null terminator character. Assume newline is at end of queue.
    queue.replace('\0', queue.getTailPos());
    // Split string into substrings delimited by space character
    // Chop up the command into segments or maximum of CLI_MAX_NUM_ARGUMENTS
    char delimiterBuffer[64];
    memset(delimiterBuffer, 0, sizeof(char)*64);
    queue.copyData((byte*) delimiterBuffer, 0, queue.getTailPos());

    char* token;
    char* rest = delimiterBuffer;
    argc = 0;
    while ((token = strtok_r(rest, " ", &rest)) && argc < CLI_MAX_NUM_ARGUMENTS) {
        strcpy(argv[argc], token);
        argc++;
    }

    auto command = parseCommand(argv[0], strlen(argv[0]));
    queue.reset();
    
    return command;
}

CliCommandIndex TurtleShell::parseCommand(const char *buffer, unsigned int bufferLength) {
    // Test if buffer is equal to cli
    // if (strncmp(buffer, cliCommands[CLI_CLI], bufferLength) == 0) { toggleShell(); }
    for (int command = 0; command < CliCommandIndex::CLI_NUM_COMMANDS; command++) { 
        
        if (strncmp(buffer, cliCommands[command], bufferLength) == 0) {
            return (CliCommandIndex) command;
        }
    }

    return CLI_NUM_COMMANDS;
}


int TurtleShell::findCommandInPacket(CircularQueue &queue, byte* buffer) {
    // Start at index 0 because that's where the start of the message will be. After each message is found
    // Loop over queue looking for matching strings
    // Get a pointer to the first ' ' character
    // Send that string to the m_shell.parseCommand() function
    // m_shell.parseCommand();    
    int finalIndex = 0;
    
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (queue[i] == ' ') {
            finalIndex = i;
        }
    }

    // printf("findCommandInPacket: delimiter @ %d\n", finalIndex);
    // Copy data out of queue into buffer
    queue.copyData(buffer, 0, finalIndex);

    return 0;
}


int TurtleShell::isNewlinePresent(CircularQueue &queue) {
    return queue.searchForCharacter('\n');
}

