#include <comms/comms.h>
#include <comms/packet_serializer.h>
#include <comms/packetID.h>
#include <drivers/bluetooth_transceiver.h>
#include <data-structures/circular_queue.h>


#include <Arduino.h>


#define BUFFER_SIZE 256


 Comms::Comms(
    std::shared_ptr<Rosbot> robot,
    std::shared_ptr<RadioInterface> rx
) : m_robot(robot),
    m_rx(rx)
{
    m_transceiver = std::make_shared<BluetoothTransceiver>(&Serial4, 9600);
    memset(m_radioChannels, 0, TX_NUM_CHANNELS*sizeof(double));
}



int Comms::run() {
    byte buffer[BUFFER_SIZE];
    const int time = millis();
    static int lastTime = 0;    

    if (time - lastTime >= 1000) {
        Serial.println("Comms");
        lastTime = time;
    }

    int numBytesInSerialBuffer = m_transceiver->isDataReady();
    
    // Return to start of loop if no data available
    if (numBytesInSerialBuffer == 0) {
        return 0;
    }

    // Read from transceiver.
    m_transceiver->readBytes(buffer, numBytesInSerialBuffer);
    
    for (int i = 0; i < numBytesInSerialBuffer; i++) {
        Serial.print( String(i) + ". " + buffer[i]);
    }
    Serial.println();

    MessageContents packet;
    packet.command = m_shell.parseCommand( (char *) buffer, numBytesInSerialBuffer);

    if (packet.command == CliCommandIndex::CLI_NUM_COMMANDS) { return 0; }


    handlePacket(packet);
    

    return 0;
}

// When a command is found inside a packet. The queue index for get next value is set to 0.
// The insert index must also be reset to 0.
int Comms::findCommandInPacket(CircularQueue queue) {
    // Start at index 0 because that's where the start of the message will be. After each message is found
    // Loop over queue looking for matching strings
    // Get a pointer to the first ' ' character
    // Send that string to the m_shell.parseCommand() function
    // m_shell.parseCommand();
    const char delimiter[2] = "-";
    
    bool isDelimiterFound = false;
    int counter = 0;
    // Where is the index starting.
    
    // Start at the 
    NextValue initialValues = queue.getNextValue();
    
    while (!isDelimiterFound) {
        

    }
    






}

int Comms::handlePacket(MessageContents packet) {
    
    switch (packet.command) {
        case (CliCommandIndex::CLI_BEGIN): {
            m_robot->ActivateControlMode();
            Serial.println("Begin");
            break;
        }

        case (CliCommandIndex::CLI_STANDBY): {
            m_robot->ActivateStandbyMode();
            Serial.println("Standby");
            break;
        }

        case (CliCommandIndex::CLI_CALIBRATE): {
            m_robot->ActivateCalibration();
            Serial.println("Calibrate");
            break;
        }

        case (CliCommandIndex::CLI_MOTOR): {
            // Send velocity commands to motor
            Serial.println("Motor command");
            break;
        }

        case (CliCommandIndex::CLI_HELP): {
            // Collect all commands and return them to the user
            Serial.println("HELP");
            break;
        }

        default:
            // Do nothing
            Serial.println("Do nothing");
            break;
    }
    
    return 0;
}




