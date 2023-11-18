#include "gtest/gtest.h"

#include "mock-objects/mock_serial_connection.h"

#include <iostream>

class TestFixtureComms : public ::testing::Test {
    void SetUp() {}

    void TearDown() {}
};

MockSerialConnection hwTransceiver;



TEST_F(TestFixtureComms, test_serial_mock_dataReady) {
    hwTransceiver.sendString("Hello");
    EXPECT_EQ(hwTransceiver.isDataReady(), true);
}

TEST_F(TestFixtureComms, test_serial_mock_ResponseReady) {
    std::string buffer = "Hello";
    hwTransceiver.sendBytes((byte *) buffer.c_str(), buffer.length()+1);
    EXPECT_EQ(hwTransceiver.isResponseReady(), true);
}

TEST_F(TestFixtureComms, test_serial_mock_read_back_string) {
    std::string buffer = "Hello";
    hwTransceiver.sendString(buffer);
    byte chbuffer[BUFFER_SIZE];
    hwTransceiver.readBytes(chbuffer, BUFFER_SIZE);
    std::string outputBuffer = (char *) chbuffer;
    EXPECT_STREQ(outputBuffer.c_str(), buffer.c_str());
    // std::cout << outputBuffer << std::endl;
}


TEST_F(TestFixtureComms, test_cli_help)