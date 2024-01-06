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