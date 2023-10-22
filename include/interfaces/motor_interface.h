#pragma once

class DcMotorInterface {
public:
    virtual void setVoltage(float voltage) = 0;
    virtual void setThrottle(int throttle) = 0;
    virtual double readCurrent() = 0;
    virtual void setDirection(bool isClockwise) = 0;
    
protected:
    int m_currThrottle;
};