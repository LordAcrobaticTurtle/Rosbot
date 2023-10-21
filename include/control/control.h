#pragma once
#include <interfaces/motor_interface.h>
#include <localisation/localisation.h>
#include <control/pid_controller.h>
#include <memory>

class Control {
public:
    Control(
        std::shared_ptr<Localisation> localisation,
        std::shared_ptr<DcMotorInterface> motorL,
        std::shared_ptr<DcMotorInterface> motorR
    );
    
    int run();

    PIDParams getParams();
    void setParams(PIDParams params);

private:

    PIDParams m_pidParams;
    std::shared_ptr<Localisation> m_localisation;
    std::shared_ptr<DcMotorInterface> m_motorL;
    std::shared_ptr<DcMotorInterface> m_motorR;



};