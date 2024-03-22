#pragma once
#include "control/pid_controller.h"

class DcMotorInterface {
public:
    // DcMotorInterface() {};
    // virtual ~DcMotorInterface() {};
    virtual void setVoltage(float voltage) = 0;
    virtual void setThrottle(int throttle) = 0;
    virtual double readCurrent() = 0;
    virtual void setDirection(bool isClockwise) = 0;
    virtual void setPosition (PIDParams &params) = 0;
    virtual void run () = 0;
    
protected:
    int m_currThrottle;
};