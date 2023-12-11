#include <stdio.h>
#include "comms/cli/turtle_shell.h"
#include <csignal>
#include <atomic>

std::atomic<bool> isAppRunning;

void sighandler(int signum) {
    isAppRunning = false;
    printf("Graceful exit\n");
}

void appendCliCommands();

int main(int argc, char** argv) {
    isAppRunning = true;
    signal(SIGINT, sighandler);

    printf("Program start\n");
    TurtleShell shell;
    CircularQueue queue;

    appendCliCommands();
    // while (isAppRunning) {
    //     // Retrieve input from stdin and process it.
    //     char buffer[256];
    //     // memset(buffer, 0, sizeof(char)*256);
    //     scanf("%s", buffer);
    //     // char ch = getchar();
    //     queue.insert(buffer, strlen(buffer));
    //     // queue.insert(ch);
    //     printf("Tail pos: %d\n", queue.getTailPos());
    //     for (int i = 0; i < queue.getTailPos() + 1 ; i++) {
    //         printf("%c", queue[i]);
    //     }
    //     printf("\n");
    //     int argc;
    //     char argv[CLI_MAX_NUM_ARGUMENTS][CLI_MAX_ARGUMENT_LENGTH];
    //     auto command = shell.searchForCommand(queue, argc, argv);

    //     if (command != CLI_NUM_COMMANDS) {
    //         printf("Command found! %d\n", command);
    //     }
    // }

    return 0;
}


void appendCliCommands() {
    byte buffer[1024] = {0};
    // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
    //  I need to put all the command into "buffer"
    byte *ptr = buffer;
    
    for (int i = 0; i < CLI_NUM_COMMANDS; i++) {
        strcpy( (char *) ptr, cliCommands[i]);    
        ptr += strlen(cliCommands[i]);
        *ptr = '\n';
        ptr += 1;
        // m_transceiver->sendBytes( (byte *) cliCommands[i], strlen(cliCommands[i]));
        // m_transceiver->sendBytes( (byte *) "\n", 1);
    }

    printf("Help string:\n%s\n", buffer);
    // m_transceiver->sendBytes( (byte *) buffer, strlen( (char*) buffer));

}