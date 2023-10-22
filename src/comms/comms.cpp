#include <comms/comms.h>


 Comms::Comms(
    std::shared_ptr<Localisation> localisation, 
    std::shared_ptr<Control> control,
    std::shared_ptr<CommsInterface> transceiver
) : m_localisation(localisation), 
    m_control(control), 
    m_transceiver(transceiver) 
{

}




