#pragma once
#include <Arduino.h>
#include <interfaces/motor_interface.h>

#define MAX_THROTTLE 255
#define MIN_THROTTLE -255

#define A_IPROPI 1000.0 // uA/A
#define R_IPROPI 2490.00 // ohms

#define K_t

#define ADC_MAX 1023
#define ADC_MIN 0

#define VOLTAGE_MAX 3.3
#define VOLTAGE_MIN 0

#define PWM_MAX 255

// Requires 3v3 signal sent to the VREF pin
class DRV8876 : public DcMotorInterface {
    public:
        DRV8876(int pinEN, int pinPH, int pinCurrSense, 
                int pinNFault, int pinSleep);
        ~DRV8876() {};
        // Clamp between 0 and 255
        virtual void setBatteryVoltage (float voltage)     override;
        virtual void setVoltage(float voltage)             override;
        virtual void setThrottle(int throttle)             override;
        virtual void setDirection(bool isClockwise)        override;
        virtual float readCurrent()                       override;
        virtual void setCurrent (double current)           override;
        virtual void run ()                                override;
        void setPosition (PIDParams &params)               override;

        void wakeup(bool setAwake);

        int readCurrentAnalog();

    private:
        void runPositionControl (PIDParams &params);

    private:
        int m_throttle; 
        bool m_isClockwise;

        int m_pinPWM;
        int m_pinDirection;
        int m_pinCurrSense;
        int m_pinNFault;
        int m_pinSleep;

        double m_current;
        PIDParams m_positionParams;

        float m_currentFilter[20];
        int m_currentInsertIndex;

};