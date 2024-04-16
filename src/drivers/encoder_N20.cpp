#include <drivers/encoder_N20.h>
#include <utility/error_codes.h>
#include <utility/timing.h>

EncoderN20::EncoderN20(int pin1, int pin2) : 
    m_pin1(pin1),
    m_pin2(pin2),
    m_encoder(pin1, pin2)
{
    m_lastCount = 0;
    m_lastReadTime = 0;
    m_radsPerSecond = 0;
    m_filterInsertIndex = 0;
}

void EncoderN20::run() {
    m_currCount = m_encoder.read();
    m_currTime = micros();
    computeRadsTravelled();
    computeRadsPerSecond();
    computeRPM();

    // Timeout check for velocity;
    // static FrequencyTimer timer(HZ_50_MICROSECONDS);
    
    // if (timer.checkEnoughTimeHasPassed()) {
    //     if (m_timeoutCountCheck == m_currCount) {
    //         m_radsPerSecond = 0;
    //         m_rpm = 0;
    //         m_lastCount = m_currCount;
    //     }
        
    //     m_timeoutCountCheck = m_currCount;
    // }

}

float EncoderN20::readRPM() {
    return m_rpm;
}

float EncoderN20::readRadsPerSecond () {
    return m_radsPerSecond;
}

float EncoderN20::readRadsTravelled () {
    return m_radsTravelled;
}

float EncoderN20::computeRadsTravelled () {
    m_radsTravelled = (float) m_currCount / CPR_WHEEL * 2 * PI;
}

float EncoderN20::computeRadsPerSecond () {
    const int countDifference = m_currCount - m_lastCount; // Could be positive or negative
    
    if (countDifference == 0) {
        return 0.0;
    }

    const int dt = m_currTime - m_lastReadTime;
    float dtInSeconds = dt * 1.0e-6;

    float currSpeed = (float) countDifference / CPR_WHEEL * 2 * PI / dtInSeconds;

    m_encoderSpeedFilter[m_filterInsertIndex++] = currSpeed;
    
    if (m_filterInsertIndex >= 19) {
        m_filterInsertIndex = 0;
    }

    // Find average of array
    float arraySum = 0;
    for (int i = 0; i < 20; i++) {
        arraySum += m_encoderSpeedFilter[i];
    }
    float average = arraySum / 20.0;
    m_radsPerSecond = average;

    m_lastReadTime = m_currTime;
    m_lastCount = m_currCount;
    return m_radsPerSecond;
}

float EncoderN20::computeRPM () {
    m_rpm = m_radsPerSecond * 60 / (2 * PI);
}

void EncoderN20::reset () {
    m_encoder.readAndReset();
    m_radsPerSecond = 0;
    m_radsTravelled = 0;
    m_currCount = 0;
}


long int EncoderN20::readPosition () {
    return m_currCount;
}

