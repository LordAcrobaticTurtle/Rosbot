// #include <stdio.h>
// #include "comms/cli/turtle_shell.h"
// #include <csignal>
// #include <atomic>

// std::atomic<bool> isAppRunning;


// struct MessageContents {
//     MessageContents() : argc(0) {}
//     int argc;
//     char argv[CLI_MAX_NUM_ARGUMENTS][CLI_MAX_ARGUMENT_LENGTH];
//     CliCommandIndex command;
// };

// void sighandler(int signum) {
//     isAppRunning = false;
//     printf("Graceful exit\n");
// }

// void appendCliCommands();
// void commsExploration();

// int main(int argc, char** argv) {
//     isAppRunning = true;
//     signal(SIGINT, sighandler);

//     printf("Program start\n");
//     TurtleShell shell;
//     CircularQueue queue;

//     // appendCliCommands();
//     commsExploration();
//     // while (isAppRunning) {
//     //     // Retrieve input from stdin and process it.
//     //     char buffer[256];
//     //     // memset(buffer, 0, sizeof(char)*256);
//     //     scanf("%s", buffer);
//     //     // char ch = getchar();
//     //     queue.insert(buffer, strlen(buffer));
//     //     // queue.insert(ch);
//     //     printf("Tail pos: %d\n", queue.getTailPos());
//     //     for (int i = 0; i < queue.getTailPos() + 1 ; i++) {
//     //         printf("%c", queue[i]);
//     //     }
//     //     printf("\n");
//     //     int argc;
//     //     char argv[CLI_MAX_NUM_ARGUMENTS][CLI_MAX_ARGUMENT_LENGTH];
//     //     auto command = shell.searchForCommand(queue, argc, argv);

//     //     if (command != CLI_NUM_COMMANDS) {
//     //         printf("Command found! %d\n", command);
//     //     }
//     // }

//     return 0;
// }


// void appendCliCommands() {
//     byte buffer[1024] = {0};
//     // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
//     //  I need to put all the command into "buffer"
//     byte *ptr = buffer;
    
//     for (int i = 0; i < CLI_NUM_COMMANDS; i++) {
//         strcpy( (char *) ptr, cliCommands[i]);    
//         ptr += strlen(cliCommands[i]);
//         *ptr = '\n';
//         ptr += 1;
//         // m_transceiver->sendBytes( (byte *) cliCommands[i], strlen(cliCommands[i]));
//         // m_transceiver->sendBytes( (byte *) "\n", 1);
//     }

//     printf("Help string:\n%s\n", buffer);
//     // m_transceiver->sendBytes( (byte *) buffer, strlen( (char*) buffer));

// }

// void switchCopy(MessageContents packet) {
        
//     switch (packet.command) {
//         case (CliCommandIndex::CLI_BEGIN): {
//             // m_robot->ActivateControlMode();

//             // m_streamMode = STREAM_MODE_CONTROL; // Send control information over the pipe. 
//             // byte buffer[] = "Begin - OK";
//             // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
//             // Serial.println(String((char*)buffer));
//             printf("Begin - OK");
//             break;
//         }

//         case (CliCommandIndex::CLI_STANDBY): {
//             // m_robot->ActivateStandbyMode();
//             // byte buffer[] = "Standby - OK";
//             // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
//             // Serial.println("Standby");
//             // m_streamMode = STREAM_MODE_NONE;
//             printf("Standby - OK");
//             break;
//         }

//         case (CliCommandIndex::CLI_CALIBRATE): {
//             printf("Calibrate - OK");
//             break;
//         }

//         case (CliCommandIndex::CLI_RESET_IMU): {
//             // m_robot->resetImu();
//             // byte buffer[] = "Reset IMU - OK";
//             // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
//             // Serial.println("Reset IMU - OK");
//             printf("Reset-IMU - OK");
//             break;
//         }

//         case (CliCommandIndex::CLI_MOTOR): {
//             // // Send velocity commands to motor
//             // byte buffer[] = "Motor - OK";
//             // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
//             // Serial.println("Motor command");
//             printf("Motor - OK");
//             break;
//         }

//         case (CliCommandIndex::CLI_HELP): {
//             // Collect all commands and return them to the user
//             // byte buffer[] = "Help: ";
//             // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));

//             // Serial.println("HELP");
//             // sendHelp();
//             printf("Help - OK");
//             break;
//         }

//         default:
//             // Do nothing
//             break;
//     }
// }



// void commsExploration() {
//     TurtleShell shell;
//     MessageContents packet;
//     CircularQueue queue;
//     char buffer[] = "Begin\n";

//     for (int j = 0; j < strlen(buffer); j++) {
//         queue.insert(buffer[j]);
//         packet.command = shell.searchForCommand(queue, packet.argc, packet.argv);
//         printf("Packet: commnad - %d, argc - %d; \n", packet.command, packet.argc);
//         for (int i = 0; i < packet.argc ;i++) {
//             printf("%s\n", packet.argv[i]);
//         }

//         switchCopy(packet);


//     }
// }