#pragma once
#include "interfaces/comms_interface.h"

#include <string>



class TestSerialConnection : public CommsInterface {
    public:
        TestSerialConnection() {};
        ~TestSerialConnection() {};

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
};


int TestSerialConnection::isDataReady() {
    return 0;
}

int TestSerialConnection::readBytes(byte* buffer, size_t bufferLength) {
    return 0;
}

int TestSerialConnection::sendBytes(byte* buffer, size_t bufferLength) {
    return 0;
}

void TestSerialConnection::sendString(std::string cliInput) {
}

int TestSerialConnection::isResponseReady() {
    return 0;
}

std::string TestSerialConnection::readString() {
    return "Hello";
}

