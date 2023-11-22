#pragma once
#include "interfaces/comms_interface.h"

#include <string>
#include <cstring>

#define BUFFER_SIZE 1024

class MockSerialConnection : public CommsInterface {
    public:
        MockSerialConnection() : m_isResponseReady(false), m_isDataReady(false) {};
        ~MockSerialConnection() {};

        /* These functions will be used BY the comms object. */
        virtual int isDataReady() override;
        virtual int readBytes(byte* buffer, size_t bufferLength) override;
        virtual int sendBytes(byte* buffer, size_t bufferLength) override;

        /**
         * @brief Load driver with mock data from test function
         * @param cliInput 
         */
        void sendString(std::string cliInput);

        /**
         * @brief Check if the internal handling has produced a response
         * @return int 
         */
        int isResponseReady();

        /**
         * @brief Reads string from internal computation.
         * @return std::string 
         */
        std::string readString();

    protected:
        bool m_isResponseReady;
        bool m_isDataReady;
        bool m_useLoopback;
        unsigned char m_commsInterfaceBuffer[BUFFER_SIZE]; // ReadBytes and sendString communicate here. 
        unsigned char m_externalBuffer[BUFFER_SIZE]; // SendBytes and readString communicates here. 

};



int MockSerialConnection::isDataReady() {
    return m_isDataReady;
}

int MockSerialConnection::readBytes(byte* buffer, size_t bufferLength) {
    if (bufferLength <= BUFFER_SIZE) {
        strncpy( (char*) buffer, (const char*) m_commsInterfaceBuffer, bufferLength);
    }
    m_isDataReady = false;
    return 0;
}

int MockSerialConnection::sendBytes(byte* buffer, size_t bufferLength) {

    strncpy((char *) m_externalBuffer, (const char *) buffer, bufferLength);
    m_isResponseReady = true;
    return 0;
}

void MockSerialConnection::sendString(std::string cliInput) {
    strcpy( (char *) m_commsInterfaceBuffer, cliInput.c_str());
    m_isDataReady = true;
}

int MockSerialConnection::isResponseReady() {
    return m_isResponseReady;
}

std::string MockSerialConnection::readString() {
    char buffer[BUFFER_SIZE];
    strcpy(buffer, (const char *) m_externalBuffer);
    std::string stringBuffer = buffer;
    m_isResponseReady  = false;
    return stringBuffer;
}

