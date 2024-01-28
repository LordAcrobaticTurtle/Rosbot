#include "utility/timing.h"
#include <Arduino.h>

HardwareTime::HardwareTime() : SystemTimeI() {

}

unsigned int HardwareTime::msElapsed () {
    return millis();
}

unsigned int HardwareTime::usElapsed () {
    return micros();
}

void setupGlobalTimer () {
    // g_systemTime = std::make_unique<HardwareTime>();
}


FrequencyTimer::FrequencyTimer () {
    m_time = 0;
    m_lastTime = 0;
    m_time = 0;
}

void FrequencyTimer::setPeriod (long int timeInMicroseconds) {
    if (timeInMicroseconds < 0) {
        return;
    }
    m_target = timeInMicroseconds;
}

bool FrequencyTimer::checkEnoughTimeHasPassed () {
    
    m_time = micros();
    if (m_time - m_lastTime < m_target) {
        return false;
    }    

    // Only update last time on successful calls. 
    m_lastTime = m_time;
    return true;
}

void FrequencyTimer::setFrequency (double timeInHz) {
    // Do something. 
}