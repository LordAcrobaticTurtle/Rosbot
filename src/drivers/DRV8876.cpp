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

    wakeup(true);
}

void DRV8876::wakeup(bool setAwake) {
    digitalWrite(m_pinSleep, setAwake);
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