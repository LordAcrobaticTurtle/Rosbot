#pragma once
#include <memory>


#include <comms/packet.h>
#include <comms/cli/turtle_shell.h>
#include <interfaces/comms_interface.h>
#include <drivers/radio_interface.h>
#include <Rosbot.h>


struct MessageContents {
    int argc;
    char **argv;
    CliCommandIndex command;
};

enum StreamMode {
    STREAM_MODE_NONE,
    STREAM_MODE_LOCALISATION_CALIBRATION,
    STREAM_MODE_CONTROL,
    STREAM_MODE_NUM_MODES
};

class Comms {
public:
    Comms(std::shared_ptr<Rosbot> robot, std::shared_ptr<RadioInterface> rx);

    int decodeStream(byte* buffer, Packet &packet);
    int run();

protected: 
    int handlePacket(MessageContents packet);

    void sendResponse(byte* buffer);

    void sendHelp();

    void returnStreamResponse();

    void sendControlResponse();

    void sendLocalisationResponse();
    // // Step 1
    // int findIdentifyingByte(byte* buffer, size_t numBytes);
    // // Step 2
    // int decodeHeader(byte* buffer, size_t numBytes, size_t packetStart, Packet &packet);
    // // Step 3
    // int decodePacket(byte* buffer, size_t numBytes, size_t packetStart, Packet &packet);

protected:
    std::shared_ptr<Rosbot> m_robot;
    std::shared_ptr<CommsInterface> m_transceiver;
    std::shared_ptr<RadioInterface> m_rx;
    CircularQueue m_commsBuffer;
    TurtleShell m_shell;

    StreamMode m_streamMode;    
    double m_radioChannels[TX_NUM_CHANNELS];
};

