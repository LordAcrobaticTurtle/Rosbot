#include <comms/comms.h>
#include <comms/packet_serializer.h>
#include <comms/packetID.h>
#include <drivers/bluetooth_transceiver.h>
#include <data-structures/circular_queue.h>


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
        for (int i = 0; i < m_commsBuffer.getTailPos() + 1; i++) {
            Serial.print(String(m_commsBuffer[i]) + " ");
        }
        lastTime = time;
    }

    int numBytesInSerialBuffer = m_transceiver->isDataReady();
    
    // Return to start of loop if no data available
    if (numBytesInSerialBuffer == 0) {
        return 0;
    }

    for (int i = 0; i < m_commsBuffer.getTailPos(); i++) {
        Serial.print(String(m_commsBuffer[i]) + " ");
    }
    // Read from transceiver.
    m_transceiver->readBytes(buffer, numBytesInSerialBuffer);
    
    m_commsBuffer.insert((const char *) buffer, numBytesInSerialBuffer);
    
    MessageContents packet;
    packet.command = m_shell.searchForCommand(m_commsBuffer);
    
    if (packet.command != CLI_NUM_COMMANDS) {
        m_commsBuffer.reset();
    }

    handlePacket(packet);
    

    return 0;
}

int Comms::handlePacket(MessageContents packet) {
    
    switch (packet.command) {
        case (CliCommandIndex::CLI_BEGIN): {
            m_robot->ActivateControlMode();
            // sendResponse("Begin - OK");
            byte buffer[] = "Begin - OK";
            m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
            break;
        }

        case (CliCommandIndex::CLI_STANDBY): {
            m_robot->ActivateStandbyMode();
            byte buffer[] = "Standby - OK";
            m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
            Serial.println("Standby");
            break;
        }

        case (CliCommandIndex::CLI_CALIBRATE): {
            m_robot->ActivateCalibration();
            byte buffer[] = "Calibration - OK";
            m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
            Serial.println("Calibrate");
            break;
        }

        case (CliCommandIndex::CLI_RESET_IMU): {
            m_robot->resetImu();
            byte buffer[] = "Reset IMU - OK";
            m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
            Serial.println("Reset IMU - OK");
            break;
        }

        case (CliCommandIndex::CLI_MOTOR): {
            // Send velocity commands to motor
            byte buffer[] = "Motor - OK";
            m_transceiver->sendBytes(buffer, strlen((const char *) buffer));
            Serial.println("Motor command");
            break;
        }

        case (CliCommandIndex::CLI_HELP): {
            // Collect all commands and return them to the user
            // byte buffer[] = "Help: ";
            // m_transceiver->sendBytes(buffer, strlen((const char *) buffer));

            // Serial.println("HELP");
            sendHelp();
            break;
        }

        default:
            // Do nothing
            Serial.println("Do nothing");
            break;
    }
    
    return 0;
}

void Comms::sendResponse(byte *buffer) {
    m_transceiver->sendBytes(buffer, strlen((const char*) buffer));
    Serial.println((char *) buffer);
}

void Comms::sendHelp() {
    Serial.println("HELP");

    byte buffer[] = "Help: ";
    m_transceiver->sendBytes(buffer, strlen((const char *) buffer));

    for (int i = 0; i < CLI_NUM_COMMANDS; i++) {
        m_transceiver->sendBytes( (byte *) cliCommands[i], strlen(cliCommands[i]));
        m_transceiver->sendBytes( (byte *) "\n", 1);
    }
}


