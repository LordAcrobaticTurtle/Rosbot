#include "gtest/gtest.h"

#include "mock-objects/mock_serial_connection.h"
#include "mock-objects/mock_robot.h"
#include "comms/cli/turtle_shell.h"
#include "comms/comms.h"

#include <iostream>

class TestFixtureComms : public ::testing::Test {
    void SetUp() {}

    void TearDown() {}
};

class TestableTurtleShell : public TurtleShell {
    public:
        TestableTurtleShell() {};
        ~TestableTurtleShell() {};

        bool getShellActive() {
            return m_isShellActive;
        }

        void testToggleShell() {
            toggleShell();
        }
};

class TestableComms : public Comms {
    public:
        TestableComms() : Comms() {}
        ~TestableComms() {}

        void replaceSerialWithMock(std::shared_ptr<MockSerialConnection> connection) {
            m_transceiver = connection;
        }
};


TEST_F(TestFixtureComms, test_serial_mock_dataReady) {
    MockSerialConnection hwTransceiver;
    hwTransceiver.sendString("Hello");
    EXPECT_EQ(hwTransceiver.isDataReady(), true);
}

TEST_F(TestFixtureComms, test_serial_mock_ResponseReady) {
    MockSerialConnection hwTransceiver;
    std::string buffer = "Hello";
    hwTransceiver.sendBytes((byte *) buffer.c_str(), buffer.length()+1);
    EXPECT_EQ(hwTransceiver.isResponseReady(), true);
}

TEST_F(TestFixtureComms, test_serial_mock_read_back_string) {
    MockSerialConnection hwTransceiver;
    std::string buffer = "Hello";
    hwTransceiver.sendString(buffer);
    byte chbuffer[BUFFER_SIZE];
    hwTransceiver.readBytes(chbuffer, BUFFER_SIZE);
    std::string outputBuffer = (char *) chbuffer;
    EXPECT_STREQ(outputBuffer.c_str(), buffer.c_str());
}


TEST_F(TestFixtureComms, test_shell_activate_deactivate) {
    TestableTurtleShell shell;
    CircularQueue queue;
    const char buffer[] = "cli ";
    queue.insert(buffer, strlen(buffer));

    shell.searchForCommand(queue);
    EXPECT_EQ(shell.getShellActive(), true);
    
    // queue.setSearchIndex(0);
    // queue.setInsertIndex(0);
    shell.searchForCommand(queue);
    EXPECT_EQ(shell.getShellActive(), false);
}

TEST_F (TestFixtureComms, test_shell_finds_correct_command) {
    CircularQueue queue;
    TurtleShell shell;
    queue.insert('c');
    queue.insert('l');
    queue.insert('i');
    queue.insert(' ');
    
    EXPECT_EQ(shell.searchForCommand(queue), CliCommandIndex::CLI_CLI);

    queue.setInsertIndex(0);
    // std::string begin = "Begin ";
    const char buffer[64] = "Begin ";
    queue.insert(buffer, strlen(buffer));
    
    EXPECT_EQ(shell.searchForCommand(queue), CliCommandIndex::CLI_BEGIN);
}

TEST_F (TestFixtureComms, test_shell_returns_every_correct_command) {
    CircularQueue queue;
    TurtleShell shell;

    for (int i = 0; i < CLI_NUM_COMMANDS; i++) {
        queue.reset();
        queue.insert(cliCommands[i], strlen(cliCommands[i]));
        queue.insert(' ');
        EXPECT_EQ(shell.searchForCommand(queue), (CliCommandIndex) i);
    }
}

TEST_F (TestFixtureComms, test_integration_of_shell_and_comms) {
    TestableComms comms;

    std::shared_ptr<MockSerialConnection> hwTransceiver = std::make_shared<MockSerialConnection>();
    comms.replaceSerialWithMock(hwTransceiver);
    

    // Place data in mock serial
    hwTransceiver->sendString("cli ");

    // Run comms and to place response packets in the serial buffer
    comms.run();


    // Pull data from mock serial and check the contents is as expected
    EXPECT_TRUE(hwTransceiver->isResponseReady());

    std::string response = hwTransceiver->readString();

    EXPECT_STREQ(response.c_str(), "CLI - OK");
}


