#pragma once
#include <comms/packet.h>
#include <localisation/localisation.h>
#include <control/control.h>
#include <interfaces/comms_interface.h>
#include <memory>

class Comms {
public:

    Comms(
        std::shared_ptr<Localisation> localisation, 
        std::shared_ptr<Control> control,
        std::shared_ptr<CommsInterface> transceiver
    );
    int decodePacket(byte* buffer, Packet &packet);
    int handlePacket(Packet packet);
    int run();

private:
    std::shared_ptr<Localisation> m_localisation;
    std::shared_ptr<Control> m_control;
    std::shared_ptr<CommsInterface> m_transceiver;
};

