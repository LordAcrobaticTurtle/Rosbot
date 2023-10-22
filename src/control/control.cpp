#include <control/control.h>


Control::Control (
    std::shared_ptr<Localisation> localisation,
    std::shared_ptr<DcMotorInterface> motorL,
    std::shared_ptr<DcMotorInterface> motorR
) : m_localisation(localisation),
    m_motorL(motorL),
    m_motorR(motorR)
{

}

