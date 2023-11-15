#pragma once
#include <Arduino.h>
#include <interfaces/encoder_interface.h>
#include <Encoder.h>
#define GEARING 100
#define PPR 7

const int enc2_c1 = 8;
const int enc2_c2 = 9;

const int enc1_c1 = 6;
const int enc1_c2 = 7;


enum EncoderReturnOptions {
    VALID,
    TIMEOUT
};

struct EncoderResult {
    EncoderReturnOptions option;
    float RPM;
};

class EncoderN20 : public EncoderInterface {

    public:
        EncoderN20(int pin1, int pin2);
        ~EncoderN20() {};
        virtual float readRPM();

        EncoderResult readRPMwithStruct();

        int setup();

        void update();

    private:
    

    private:
        const int pulsesPerRevolution = 7;
        
        long int m_lastCount;
        long int m_lastReadTime;
        
        int m_pin1, m_pin2;

        Encoder m_encoder;
};