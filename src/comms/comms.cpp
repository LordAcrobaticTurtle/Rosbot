#include <comms/comms.h>
#include <comms/packet_serializer.h>
#include <drivers/bluetooth_transceiver.h>
#include <data-structures/circular_queue.h>
#include <utility/timing.h>


#define BUFFER_SIZE 256


Comms::Comms(
    std::shared_ptr<Rosbot> robot,
    std::shared_ptr<RadioInterface> rx
) : m_robot(robot),
    m_rx(rx),
    m_streamMode(STREAM_MODE_NONE)
{
    m_transceiver = std::make_shared<BluetoothTransceiver>(&Serial4, 115200);
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
    
    serialHeartbeat();

    int numBytesInSerialBuffer = m_transceiver->isDataReady();
    
    // Return to start of loop if no data available
    if (numBytesInSerialBuffer > 0) {    
        byte buffer[BUFFER_SIZE];
        m_transceiver->readBytes(buffer, numBytesInSerialBuffer);
        m_commsBuffer.insert((const char *) buffer, numBytesInSerialBuffer);
    
        for (int i = 0; i < m_commsBuffer.getTailPos() + 1; i++) {
            Serial.println(m_commsBuffer[i]);
        }
    }

    MessageContents packet;
    packet.command = m_shell.searchForCommand(m_commsBuffer, packet.argc, packet.argv);

    handlePacket(packet);
    
    returnStreamResponse();

    return 0;
}

int Comms::handlePacket(MessageContents packet) {
    
    switch (packet.command) {
        case (CliCommandIndex::CLI_BEGIN): {
            m_robot->ActivateControlMode();

            m_streamMode = STREAM_MODE_CONTROL; // Send control information over the pipe. 
            byte buffer[] = "Begin-OK";
            sendResponse(buffer, CliCommandIndex::CLI_BEGIN);
            Serial.println(String((char*)buffer));
            break;
        }

        case (CliCommandIndex::CLI_STANDBY): {
            m_robot->ActivateStandbyMode();
            byte buffer[] = "Standby-OK";
            sendResponse(buffer, CliCommandIndex::CLI_STANDBY);
            Serial.println("Standby");
            m_streamMode = STREAM_MODE_NONE;
            break;
        }

        case (CliCommandIndex::CLI_CALIBRATE): {
            m_robot->ActivateCalibration();
            m_streamMode = STREAM_MODE_LOCALISATION_CALIBRATION;
            byte buffer[] = "Calibration-OK";
            sendResponse(buffer, CliCommandIndex::CLI_CALIBRATE);
            // Capture current time to use as offset
            m_timerOffset = millis();
            Serial.println("Calibrate");
            break;
        }

        case (CliCommandIndex::CLI_RESET_IMU): {
            m_robot->resetImu();
            byte buffer[] = "Reset_IMU-OK";
            sendResponse(buffer, CliCommandIndex::CLI_RESET_IMU);
            Serial.println("Reset IMU-OK");
            break;
        }

        case (CliCommandIndex::CLI_MOTOR): {
            // Send velocity commands to motor
            byte buffer[] = "Motor-OK";
            sendResponse(buffer, CliCommandIndex::CLI_MOTOR);
            Serial.println("Motor command");
            break;
        }

        case (CliCommandIndex::CLI_HELP): {
            // Collect all commands and return them to the user
            sendHelp();
            break;
        }

        default:
            // Do nothing
            break;
    }
    return 0;
}

void Comms::sendResponse(byte *buffer, CliCommandIndex packetID) {
    // Frame data with null bytes
    byte bufferToSend[256];
    auto time = millis();
    // sprintf((char*) bufferToSend, "0x%x %d %ld %s 0x%x", FRAMING_START, packetID, time - m_timerOffset, buffer, FRAMING_END);
    m_transceiver->sendBytes(bufferToSend, strlen((const char*) bufferToSend));
    // Serial.println((char *) bufferToSend);
}

void Comms::sendHelp() {
    byte buffer[256] = {0};
    byte *ptr = buffer;
    for (int i = 0; i < CLI_NUM_COMMANDS; i++) {
        strcpy( (char *) ptr, cliCommands[i]);    
        ptr += strlen(cliCommands[i]);
        *ptr = '\n';
        ptr += 1;
    }
    
    sendResponse(buffer, CliCommandIndex::CLI_HELP);
}

void Comms::returnStreamResponse() {
    // Check what type of mode is being used. 
    // Generate a response and send it to the transceiver interface. 

    // Is this where I'll pack it into a json object or nah. Yes but inside another function
    const long int time = millis();
    static long int timeElapsed = 0;

    switch (m_streamMode) {
        case STREAM_MODE_CONTROL: {
            sendControlResponse();
            // Serial.println("Control response");
            break;
        }

        case STREAM_MODE_LOCALISATION_CALIBRATION: {
            // Pre-calculate number of bytes required for 11 floats, but as a string 
            sendLocalisationResponse();
            // Serial.println("Localisation response");
            break;
        }

        default: 
            break;
    }
    
}

void Comms::serialHeartbeat() {
    const unsigned int time = millis();
    static unsigned int lastTime = 0;    
    
    if (time - lastTime >= 1000) {
        Serial.println("Comms");
        // for (int i = 0; i < m_commsBuffer.getTailPos() + 1; i++) {
        //     Serial.print(String(m_commsBuffer[i]) + " ");
        // }
        lastTime = time;
    }
}

void Comms::sendControlResponse() {
    ControlResponse res = m_robot->getControlResponse();
    
    char buffer[1028] = {0};
    char paramBuffer[256] = {0};
    res.params.toString(paramBuffer);
    sprintf(buffer, "%s", paramBuffer);
    sendResponse((byte*) buffer, CLI_CONTROL_PACKET);
}

void Comms::sendLocalisationResponse() {
    LocalisationResponse res = m_robot->getLocalisationResponse();
    byte buffer[1028];
    
    char accelBuffer[128] = {0};
    res.accelReadings.toString(accelBuffer);

    char gyroBuffer[128] = {0};
    res.gyroRates.toString(gyroBuffer);

    char angleBuffer[128] = {0};
    res.orientation.toString(angleBuffer);

    char vwheelBuffer[128] = {0};
    res.encoderVelocities.toString(vwheelBuffer);

    sprintf((char *) buffer, "%s,%s,%s,%s,(%f)", accelBuffer, gyroBuffer, angleBuffer, vwheelBuffer, 0);
    sendResponse(buffer, CliCommandIndex::CLI_LOCALISATION_PACKET);
     
}