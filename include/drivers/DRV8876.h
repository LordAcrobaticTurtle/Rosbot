#pragma once
#include <Arduino.h>

#define MAX_THROTTLE 255
#define MIN_THROTTLE -255

#define A_IPROPI 1000 // uA/A
#define R_IPROPI 1500 // ohms

#define K_t

#define ADC_MAX 1023
#define ADC_MIN 0

#define VOLTAGE_MAX 3.3
#define VOLTAGE_MIN 0

#define PWM_MAX 255

// Requires 3v3 signal sent to the VREF pin
class DRV8876 {
    public:
        DRV8876(int pinEN, int pinPH, int pinCurrSense, 
                int pinNFault, int pinSleep);

        // Clamp between 0 and 255
        void setThrottle(int throttle);
        void setDirection(bool isClockwise);
        void wakeup(bool setAwake);

        /** Read current is tricky. Greatly depends on what state the driver is
        * in. Fairly certain it is only useful when actively driving or braking
        * Returns current in Amps
        */ 
        double readCurrent();

    private:


    private:
        int m_throttle; 
        bool m_isClockwise;

        int m_pinPWM;
        int m_pinDirection;
        int m_pinCurrSense;
        int m_pinNFault;
        int m_pinSleep;

};