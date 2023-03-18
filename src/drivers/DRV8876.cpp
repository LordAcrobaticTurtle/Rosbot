#include <drivers/DRV8876.h>


DRV8876::DRV8876(
    int pinEN, int pinPH, int pinCurrSense, 
    int pinNFault, int pinSleep
) {
    m_pinPWM = pinEN;
    m_pinDirection = pinPH;
    m_pinCurrSense = pinCurrSense;
    m_pinNFault = pinNFault;
    m_pinSleep = pinSleep;
}

void DRV8876::wakeup(bool setAwake) {
    digitalWrite(m_pinSleep, setAwake);
}

void DRV8876::setDirection(bool isClockwise) {
    digitalWrite(m_pinDirection, isClockwise);
    m_isClockwise = isClockwise;
}