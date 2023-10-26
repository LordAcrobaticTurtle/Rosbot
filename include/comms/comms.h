#pragma once
#include <memory>


#include <comms/packet.h>
#include <interfaces/comms_interface.h>
#include <drivers/radio_interface.h>
#include <Rosbot.h>


class Comms {
public:
    Comms(std::shared_ptr<Rosbot> robot, std::shared_ptr<RadioInterface> rx);

    int decodeStream(byte* buffer, Packet &packet);
    int handlePacket(Packet packet);
    int run();

private: 
    // Step 1
    int findIdentifyingByte(byte* buffer, size_t numBytes);
    // Step 2
    int decodeHeader(byte* buffer, size_t numBytes, size_t packetStart, Packet &packet);
    // Step 3
    int decodePacket(byte* buffer, size_t numBytes, size_t packetStart, Packet &packet);


private:
    // std::shared_ptr<Localisation> m_localisation;
    // std::shared_ptr<Control> m_control;
    std::shared_ptr<Rosbot> m_robot;
    std::shared_ptr<CommsInterface> m_transceiver;
    std::shared_ptr<RadioInterface> m_rx;
    double m_radioChannels[TX_NUM_CHANNELS];
};

