
#include <Arduino.h>
#include "drivers/RGB_LED.h"

RGBLED::RGBLED(int pinRed, int pinGreen, int pinBlue, bool isCommonGnd) {
    m_pinRed = pinRed;
    m_pinGreen = pinGreen;
    m_pinBlue = pinBlue;
    m_isCommonGnd = isCommonGnd;

    // If common gnd pin, need a logical one to switch on and a logical zero to switch off
    // Else we have a common positive. So to switch LEDs on, need a logical zero
    if (m_isCommonGnd) {
        m_onState = HIGH;
        m_offState = LOW;
    } else {
        m_onState = LOW;
        m_offState = HIGH;
    }

    pinMode(m_pinRed, OUTPUT);
    pinMode(m_pinGreen, OUTPUT);
    pinMode(m_pinBlue, OUTPUT);
}

void RGBLED::switchRedOn() {
    setCurrentColour(255,0,0);
    digitalWrite(m_pinBlue, m_offState);
    digitalWrite(m_pinGreen, m_offState);
    digitalWrite(m_pinRed, m_onState);
}

void RGBLED::switchGreenOn() {
    setCurrentColour(0,255,0);
    digitalWrite(m_pinBlue, m_offState);
    digitalWrite(m_pinGreen, m_onState);
    digitalWrite(m_pinRed, m_offState);
}

void RGBLED::switchBlueOn() {
    setCurrentColour(0, 0, 255);
    digitalWrite(m_pinBlue, m_onState);
    digitalWrite(m_pinGreen, m_offState);
    digitalWrite(m_pinRed, m_offState);
}

void RGBLED::switchOff() {
    setCurrentColour(0, 0, 0);
    digitalWrite(m_pinBlue, m_offState);
    digitalWrite(m_pinGreen, m_offState);
    digitalWrite(m_pinRed, m_offState);
}

void RGBLED::mix(int red, int green, int blue) {
    setCurrentColour(red, green, blue);
    analogWrite(m_pinRed, red);
    analogWrite(m_pinGreen, green);
    analogWrite(m_pinBlue, blue);
}

void RGBLED::setCurrentColour(int red, int green, int blue) {
    m_currentColour[0] = red;
    m_currentColour[1] = green;
    m_currentColour[2] = blue;
}