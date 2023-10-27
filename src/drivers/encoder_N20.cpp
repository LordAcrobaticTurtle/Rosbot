#include <drivers/encoder_N20.h>
#include <utility/error_codes.h>


EncoderN20::EncoderN20(int pin1, int pin2) : m_encoder(pin1, pin2)
{

}

int EncoderN20::setup() {

    
    // attachInterrupt(enc1_c1, encoder1_c1_callback, CHANGE);
    // attachInterrupt(enc1_c2, encoder1_c2_callback, CHANGE);


    attachInterrupt(enc2_c1, encoder1_c1_callback, CHANGE);
    // attachInterrupt(enc2_c2, encoder1_c2_callback, RISING);
}

void EncoderN20::update() {
    
    Serial.println("Encoder Count: " + String(enc1Count));
    // Do computation for velocity here
}

float EncoderN20::readRPM() {
    // Check the last time the encoder was read. 
    
    long int count = m_encoder.read();
    const int currTime = micros();
    const int dt = currTime - m_lastReadTime;
    const int countDifference = count - m_lastCount; // Could be positive or negative

    float dtInSeconds = dt / 1000000.0;

    float RPM = countDifference / dtInSeconds;

    m_lastReadTime = currTime;
    m_lastCount = count;
    return RPM;
}

