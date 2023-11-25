#pragma once
#include <memory>


#include <comms/packet.h>
#include <comms/cli/turtle_shell.h>
#include <interfaces/comms_interface.h>
// #include <drivers/radio_interface.h>
#include <Rosbot.h>


struct MessageContents {
    int argc;
    char **argv;
    CliCommandIndex command;
};

class Comms {
public:
    Comms(std::shared_ptr<Rosbot> robot/*, std::shared_ptr<RadioInterface> rx*/);

    int decodeStream(byte* buffer, Packet &packet);
    int run();

protected: 
    int handlePacket(MessageContents packet);

    void sendResponse(byte* buffer);

    void sendHelp();
    
protected:
    std::shared_ptr<Rosbot> m_robot;
    std::shared_ptr<CommsInterface> m_transceiver;
    // std::shared_ptr<RadioInterface> m_rx;
    CircularQueue m_commsBuffer;
    TurtleShell m_shell;
    
    // double m_radioChannels[TX_NUM_CHANNELS];
};

