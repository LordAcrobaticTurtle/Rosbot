#pragma once
#include "control/pid_controller.h"
#include "interfaces/encoder_interface.h"
#include <memory>

class DcMotorInterface {
public:
    /**
     * @brief Sets the voltage output on the motor. 
     * @param voltage Valid values are [0, +/- batteryVoltageMax]
     */
    virtual void setVoltage(float voltage) = 0;
    
    /**
     * @brief Sets the PWM duty cycle on the motor. 
     * @param throttle [0, +/- 255]
     */
    virtual void setThrottle(int throttle) = 0;
    
    virtual double readCurrent() = 0;
    
    /**
     * @brief Sets the class variable current to be used in torque estimation.
     * This function was only added to sidestep the hardware mistake of mapping an analog signal to a 
     * digital pin
     * @param current 
     */
    virtual void setCurrent (double current) = 0;
    
    /**
     * @brief Sets voltage for the battery. Useful for the "setVoltage()" function. Must be a positive number
     * @param voltage 
     */
    virtual void setBatteryVoltage (float voltage);

    virtual void setDirection(bool isClockwise) = 0;
    
    virtual void setPosition (PIDParams &params) = 0;

    virtual void run () = 0;

protected:
    int m_currThrottle;
    std::shared_ptr<EncoderInterface> m_encoder;
    float m_targetVoltage;
    float m_batteryVoltage;
};