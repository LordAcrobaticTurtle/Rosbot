#include <comms/comms.h>
#include <comms/packet_serializer.h>
#include <drivers/bluetooth_transceiver.h>
#include <data-structures/circular_queue.h>
#include <utility/timing.h>


#define BUFFER_SIZE 256


Comms::Comms(
    std::shared_ptr<Rosbot> robot
) : m_robot(robot),
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
        

        for (int i = 0; i < m_commsBuffer.getTailPos(); i++) {
            Serial.print((char) m_commsBuffer[i]);
        }
    }

    MessageContents packet;
    packet.command = m_shell.searchForCommand(m_commsBuffer, packet.argc, packet.argv);

    handlePacket(packet);
    
    returnStreamResponse();

    return 0;
}

int Comms::handlePacket(MessageContents packet) {

    if (packet.argc < 1) {
        return -1;
    }

    switch (packet.command) {
        case (CliCommandIndex::CLI_BEGIN): {
            m_robot->ActivateControlMode();

            m_streamMode = STREAM_MODE_MODEL_CONTROL; // Send control information over the pipe. 
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
            vector3D angleOffsets = m_robot->getAngleOffsets(); 
            // 3 floats at 6 point precision capped at 2 ish places
            char angleBuffer[64];
            angleOffsets.toString(angleBuffer);
            byte msgOk[] = "Reset_IMU-OK";
            char buffer[128];
            sprintf(buffer, "%s,%s", msgOk, angleBuffer);
            sendResponse( (byte *) buffer, CliCommandIndex::CLI_RESET_IMU);
            Serial.println("Reset IMU-OK");
            break;
        }

        case (CliCommandIndex::CLI_MOTOR): {
            // Send velocity commands to motor
            int motorIndex = -1;
            int throttle = 0;

            if (packet.argc != 2) {
                byte buffer[] = "cli-Motor-Not-Ok. Argc != 2";
                sendResponse(buffer, CLI_MOTOR);
                return 0;
            }

            int valuesFilled = sscanf(packet.argv[1], "[%d,%d]", &motorIndex, &throttle);

            if (valuesFilled != 2) {
                byte buffer[] = "cli-Motor-Not-Ok. ValuesFilled != 2";
                sendResponse(buffer, CLI_MOTOR);
                return 0;
            }
            
            m_robot->setMotorPosition(motorIndex, throttle);
            // auto params = m_robot->getPositionPIDParams();
            // params.target = throttle;
            // m_robot->setPositionPIDParams(params);

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

        case (CliCommandIndex::CLI_PID_PARAMS_GET): {
            // Return PID params
            PIDParams params = m_robot->getAnglePIDParams();
            char buffer[128];
            params.toString(buffer);
            sendResponse((byte *) buffer, CLI_PID_PARAMS_GET);
            break;
        }

        case (CliCommandIndex::CLI_PID_PARAMS_SET): {
            // Parse and set PID params
            float p = 0; 
            float i = 0;
            float d = 0;
            int controlIndex = 0;
            if (packet.argc != 2) {
                byte buffer[] = "Set-pid-Not-Ok. Argc != 2";
                sendResponse(buffer, CLI_PID_PARAMS_SET);
                return 0;
            }

            int valuesFilled = sscanf(packet.argv[1], "[%d,%f,%f,%f]", &controlIndex, &p, &i, &d);
            
            if (valuesFilled != 4) {
                byte buffer[] = "Set-pid-Not-Ok. valuesFilled != 4";
                sendResponse(buffer, CLI_PID_PARAMS_SET);
                return 0;
            }

            if (controlIndex == 0) {
                PIDParams params = m_robot->getAnglePIDParams();
                params.kd = d;
                params.kp = p;
                params.ki = i;
                m_robot->setAnglePIDParams(params);
            } else if (controlIndex == 1) {
                PIDParams params = m_robot->getPositionPIDParams();
                params.kd = d;
                params.kp = p;
                params.ki = i;
                m_robot->setPositionPIDParams(params);
            }

            byte buffer[] = "Set-pid-Ok";
            sendResponse(buffer, CLI_PID_PARAMS_SET);
            Serial.println("Set-pid Ok");
            break;
        }

        case (CliCommandIndex::CLI_ANGLE_TARGET): {
            float angleTarget = 0.0;
            int valuesFilled = sscanf(packet.argv[1], "[%f]", &angleTarget);

            if (valuesFilled != 1) {
                byte buffer[] = "Set-angle-target-NOT-Ok";
                sendResponse(buffer, CLI_PID_PARAMS_SET);    
            }
            auto params = m_robot->getAnglePIDParams();
            params.target = angleTarget;
            m_robot->setAnglePIDParams(params);

            byte buffer[] = "Set-angle-target-Ok";
            sendResponse(buffer, CLI_PID_PARAMS_SET);
            break;
        }

        case (CliCommandIndex::CLI_SENSOR_VERIFICATION): {
            // Unpackage arguments
            float time = 0.0;
            float throttle = 0.0;
            int motorIndex = 0;

            int valuesFilled = sscanf(packet.argv[1], "[%d,%f,%f]", &motorIndex, &throttle, &time);

            if (valuesFilled != 3) {
                byte buffer[] = "Sensor-Verification-NOT-Ok";
                sendResponse(buffer, CLI_SENSOR_VERIFICATION);   
                return -1;
            }

            VerifiedSensorData data = m_robot->sensorVerification(motorIndex, throttle, time);
            byte dataBuffer[256];
            data.toString( (char *) dataBuffer);
            sendResponse(dataBuffer, CLI_SENSOR_VERIFICATION);

            byte okBuffer[256] = "Sensor-verification-OK";
            sendResponse(okBuffer, CLI_SENSOR_VERIFICATION);
            break;
        }

        default:
            // Do nothing
            byte notOkBuffer[256];
            sprintf((char *) notOkBuffer, "Packet-Not-Recognised: %d, %d", (int) packet.command, packet.argc);
            sendResponse(notOkBuffer, CLI_CLI);
            break;
    }
    return 0;
}

void Comms::sendResponse(byte *buffer, CliCommandIndex packetID) {
    // Frame data with null bytes
    byte bufferToSend[256];
    auto time = millis();
    sprintf((char*) bufferToSend, "0x%x %d %ld [%s] 0x%x", FRAMING_START, packetID, time - m_timerOffset, buffer, FRAMING_END);
    m_transceiver->sendBytes(bufferToSend, strlen((const char*) bufferToSend));
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

    switch (m_streamMode) {
        case STREAM_MODE_CONTROL: {
            sendControlResponse();
            break;
        }

        case STREAM_MODE_LOCALISATION_CALIBRATION: {
            // Pre-calculate number of bytes required for 11 floats, but as a string 
            sendLocalisationResponse();
            break;
        }

        case STREAM_MODE_MODEL_CONTROL: {
            sendModelControlResponse();
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

void Comms::sendModelControlResponse () {
    ModelControlResponse res = m_robot->getModelControlResponse();
    // How many digits per float? 6 sig figs???
    byte buffer[128];
    sprintf( (char *) buffer, 
        "%f,%f,%f,%f,%f,%f", 
        res.inputForce, 
        res.desiredVelocity,
        res.stateBuffer[0], 
        res.stateBuffer[1],
        res.stateBuffer[2], 
        res.stateBuffer[3]
    );

    sendResponse(buffer, CliCommandIndex::CLI_CONTROL_PACKET);
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

    char pwheelBuffer[128] = {0};
    res.encoderPositions.toString(pwheelBuffer);

    sprintf((char *) buffer, "%s,%s,%s,%s,%s", accelBuffer, gyroBuffer, angleBuffer, pwheelBuffer, vwheelBuffer);
    sendResponse(buffer, CliCommandIndex::CLI_LOCALISATION_PACKET);
     
}