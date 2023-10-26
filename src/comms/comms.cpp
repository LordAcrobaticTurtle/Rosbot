#include <comms/comms.h>
#include <comms/packet_serializer.h>
#include <comms/packetID.h>

#define BUFFER_SIZE 256

 Comms::Comms(
    std::shared_ptr<Rosbot> robot,
    std::shared_ptr<RadioInterface> rx
) : m_robot(robot),
    m_rx(rx)
{
    
    memset(m_radioChannels, 0, TX_NUM_CHANNELS*sizeof(double));
    m_rx->setup();
}



int Comms::run() {
    


    byte array[BUFFER_SIZE];
    if (m_transceiver->isDataReady()) {
        // Could face problems with timeouts cutting off incoming packets
        m_transceiver->readBytes(array, BUFFER_SIZE);
        Packet packet;
        PacketSerializer::deserialize(array, BUFFER_SIZE, packet);
        
        handlePacket(packet);

    }
}


/**
 * @brief 
 * 1. On receiving a BEGIN packet. The robot should start running its update functions. In this case, 
 * that's localisation and control loops
 * 2. On receiving a standby packet, the robot should cease running its update function and only 
 * listen for comms requests
*/

int Comms::handlePacket(Packet packet) {

    switch (packet.m_header.m_packetID) {
        
        case (PacketID::BEGIN): {
            // How to 
            m_robot->switchStandbyMode(false);
            break;
        }

        case (PacketID::STANDBY): {
            m_robot->switchStandbyMode(true);
            break;
        }

        case (PacketID::ESTOP): {
            break;
        }

        case (PacketID::REQUEST): {
            break;
        }

        case (PacketID::STATE): {
            break;
        }

        case (PacketID::ESTIMATE_BIAS): {
            break;
        }

        case (PacketID::LED_CHANGE): {
            break;
        }   

        

    }
}




