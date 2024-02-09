#include <drivers/encoder_N20.h>
#include <utility/error_codes.h>

EncoderN20::EncoderN20(int pin1, int pin2) : 
    m_pin1(pin1),
    m_pin2(pin2),
    m_encoder(pin1, pin2)
{
    m_lastCount = 0;
    m_lastReadTime = 0;
}

void EncoderN20::run() {
    m_currCount = m_encoder.read();
    computeRPM();
    m_lastCount = m_currCount;
}

float EncoderN20::readRPM() {
    return m_rpm;
}

float EncoderN20::computeRPM () {

    m_currTime = micros();
    const int dt = m_currTime - m_lastReadTime;
    const int countDifference = m_currCount - m_lastCount; // Could be positive or negative

    float dtInSeconds = dt * 1.0e-6;

    float RPS = countDifference / (dtInSeconds*PPR); 
    m_rpm = RPS *60 ;

    m_lastReadTime = m_currTime;
}

long int EncoderN20::readPosition () {
    return m_currCount;
}

