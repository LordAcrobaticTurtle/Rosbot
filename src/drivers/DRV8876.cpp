#include <drivers/DRV8876.h>
#include <utility/math.h>

DRV8876::DRV8876(
    int pinEN, int pinPH, int pinCurrSense, 
    int pinNFault, int pinSleep
) {
    m_pinPWM = pinEN;
    m_pinDirection = pinPH;
    m_pinCurrSense = pinCurrSense;
    m_pinNFault = pinNFault;
    m_pinSleep = pinSleep;

    pinMode(m_pinPWM, OUTPUT);
    pinMode(m_pinDirection, OUTPUT);
    pinMode(m_pinCurrSense, INPUT);
    pinMode(m_pinNFault, OUTPUT);
    pinMode(m_pinSleep, OUTPUT);
    wakeup(true);
    
    setThrottle(0);
    setThrottle(0);
}

void DRV8876::wakeup(bool setAwake) {
    digitalWrite(m_pinSleep, setAwake);
}

void DRV8876::setThrottle(int throttle) {
    int throttleSet = throttle;
    if (throttleSet < 0) {
        throttleSet = 0;
    }

    if (throttleSet > PWM_MAX) {
        throttleSet = PWM_MAX;
    }
    
    analogWrite(m_pinPWM, throttleSet);
}

void DRV8876::setDirection(bool isClockwise) {
    digitalWrite(m_pinDirection, isClockwise);
    m_isClockwise = isClockwise;
}

double DRV8876::readCurrent() {

    int currentAsInteger = analogRead(m_pinCurrSense);

    // Convert to voltage
    double currentAsVoltage = floatMap(currentAsInteger, ADC_MIN, ADC_MAX, VOLTAGE_MIN, VOLTAGE_MAX);
    double I_propI = currentAsVoltage / R_IPROPI; // microamps
    return I_propI / A_IPROPI; // Amps
}