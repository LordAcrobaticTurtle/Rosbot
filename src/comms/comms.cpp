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



// Expected behaviour.
// Copy bytes from serial port into byte buffer.
// Insert bytes into m_commsBuffer circular queue. 
// Commands will be delimited by ' ' chars
// Once a command is FOUND, queue will be reset
// How to find a command? 
    // Get pointer to first element. Get pointer to first delimiter
    // Copy bytes between into a char buffer, and give it to shell.parseCommand(). Ensure buffer is null terminated.
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
        // Serial.print( String(i) + ". " + buffer[i]);
        m_commsBuffer.insert(buffer[i]);
    }
    // Serial.println();

    MessageContents packet;
    // Set search index to start of queue.
    // m_commsBuffer.setSearchIndex(0);
    // findCommandInPacket(m_commsBuffer);
    // packet.command = m_shell.parseCommand(m_commsBuffer);

    handlePacket(packet);
    

    return 0;
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




