// #include <stdio.h>
// #include "comms/cli/turtle_shell.h"
// #include <csignal>
// #include <atomic>

// std::atomic<bool> isAppRunning;

// void sighandler(int signum) {
//     isAppRunning = false;
//     printf("Graceful exit\n");
// }

// int main(int argc, char** argv) {
//     isAppRunning = true;
//     signal(SIGINT, sighandler);

//     printf("Program start\n");
//     TurtleShell shell;
//     CircularQueue queue;
//     while (isAppRunning) {
//         // Retrieve input from stdin and process it.
//         char buffer[256];
//         // memset(buffer, 0, sizeof(char)*256);
//         scanf("%s", buffer);
//         // char ch = getchar();
//         queue.insert(buffer, strlen(buffer));
//         // queue.insert(ch);
//         printf("Tail pos: %d\n", queue.getTailPos());
//         for (int i = 0; i < queue.getTailPos() + 1 ; i++) {
//             printf("%c", queue[i]);
//         }
//         printf("\n");
//         int argc;
//         char argv[CLI_MAX_NUM_ARGUMENTS][CLI_MAX_ARGUMENT_LENGTH];
//         auto command = shell.searchForCommand(queue, argc, argv);

//         if (command != CLI_NUM_COMMANDS) {
//             printf("Command found! %d\n", command);
//         }
//     }

//     return 0;
// }