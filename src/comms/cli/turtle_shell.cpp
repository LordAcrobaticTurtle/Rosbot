#include "comms/cli/turtle_shell.h"

CliCommandIndex TurtleShell::searchForCommand(CircularQueue &queue) {
    
    byte buffer[32];
    memset(buffer,0,sizeof(byte)*32);
    findCommandInPacket(queue, buffer);
    size_t bufferLength = strlen( (const char*) buffer);

    // Remove do not consider the space in the buffer. Remove 1
    // printf("searchForCommand: %s, %d\n", buffer, bufferLength);
    return parseCommand((const char*)buffer, bufferLength-1);

}

CliCommandIndex TurtleShell::parseCommand(const char *buffer, unsigned int bufferLength) {
    // Test if buffer is equal to cli
    if (strncmp(buffer, cliCommands[CLI_CLI], bufferLength) == 0) { toggleShell(); }
    
    if (!m_isShellActive) {
        return CLI_NUM_COMMANDS;
    }

    for (int command = 0; command < CliCommandIndex::CLI_NUM_COMMANDS; command++) { 
        
        if (strncmp(buffer, cliCommands[command], bufferLength) == 0) {
            return (CliCommandIndex) command;
        }
    }

    return CLI_NUM_COMMANDS;
}

// When a command is found inside a packet. The queue index for get next value is set to 0.
// The insert index must also be reset to 0.
int TurtleShell::findCommandInPacket(CircularQueue &queue, byte* buffer) {
    // Start at index 0 because that's where the start of the message will be. After each message is found
    // Loop over queue looking for matching strings
    // Get a pointer to the first ' ' character
    // Send that string to the m_shell.parseCommand() function
    // m_shell.parseCommand();    
    bool isDelimiterFound = false;
    int finalIndex = 0;
    int validCounter = 0;
    // Where is the index starting.
    
    // Start at the 
    NextValue initialValues = queue.getNextValue();
    
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (queue[i] == ' ') {
            finalIndex = i;
        }
    }

    // printf("findCommandInPacket: delimiter @ %d\n", finalIndex);
    // Copy data out of queue into buffer
    queue.copyData(buffer, 0, finalIndex);
}
