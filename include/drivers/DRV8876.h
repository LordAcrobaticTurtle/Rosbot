#pragma once
#include <Arduino.h>

#define MAX_THROTTLE 255

class DRV8876 {
    public:
        DRV8876(int pinEN, int pinPH, int pinCurrSense, 
                int pinNFault, int pinSleep);

        // Clamp between 0 and 255
        void setThrottle(int throttle);
        void setDirection(bool isClockwise);
        void wakeup(bool setToSleep);

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