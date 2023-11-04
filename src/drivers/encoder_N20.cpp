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

int EncoderN20::setup() {

    
    // attachInterrupt(enc1_c1, encoder1_c1_callback, CHANGE);
    // attachInterrupt(enc1_c2, encoder1_c2_callback, CHANGE);


    // attachInterrupt(enc2_c1, encoder1_c1_callback, CHANGE);
    // attachInterrupt(enc2_c2, encoder1_c2_callback, RISING);
    return 0;
}

void EncoderN20::update() {
    
    // Serial.print(", Encoder + " + String(m_pin1) + ", Count: " + String(m_encoder.read()));
    // Do computation for velocity here
}

float EncoderN20::readRPM() {
    // Check the last time the encoder was read. 
    
    long int count = m_encoder.read();
    const int currTime = micros();
    // 500us
    if (currTime - m_lastCount <= 1000000) {
        return -1;
    }
    const int dt = currTime - m_lastReadTime;
    const int countDifference = count - m_lastCount; // Could be positive or negative

    float dtInSeconds = dt / 1000000.0; // s

    float RPS = countDifference / (dtInSeconds*PPR); 
    float RPM = RPS *60 ;

    m_lastReadTime = currTime;
    m_lastCount = count;
    return RPM;
}

EncoderResult EncoderN20::readRPMwithStruct() {
    EncoderResult res;
    const int period_us = 100000.0;
    long int count = m_encoder.read();
    const int currTime = micros();
    
    if (currTime - m_lastReadTime < period_us) {
        res.option = TIMEOUT;
        res.RPM = 0;
        return res;
    }

    const int dt = currTime - m_lastReadTime;
    m_lastReadTime = currTime;

    if (dt == 0) {
        res.option = TIMEOUT;
        res.RPM = 0;
        return res;
    }
    const int countDifference = count - m_lastCount; // Number of pulses in one period


    float dtInSeconds = dt / 100000.0; // s

    float RPS = countDifference / (7.0*100.0); 

    m_lastCount = count;
    res.option = VALID;
    res.RPM = RPS*60*2.5;
    // Serial.println("RPM: " + String(RPM) + ", dt: " + String(dt));
    return res;



}

