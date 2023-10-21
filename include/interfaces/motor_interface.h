#pragma once

class DcMotorInterface {
public:
    virtual void setVoltage(float voltage) = 0;
    virtual void setThrottle(int throttle) = 0;
    virtual float readCurrent() = 0;

protected:
    int m_currThrottle;
};