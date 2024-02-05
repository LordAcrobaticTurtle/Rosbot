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


class EncoderN20 : public EncoderInterface {

    public:
        EncoderN20(int pin1, int pin2);
        ~EncoderN20() {};

        virtual float readRPM ()         override;
        virtual long int readPosition () override;

        // This function assumes you are calling with a reasonable amount of time in between each function calls. 
        // I.e. On the scale of milliseconds, not microseconds
        virtual void update ()           override;


    private:
        float computeRPM ();

    private:
        const int pulsesPerRevolution = 7;
        
        float m_rpm;

        long int m_currCount;
        long int m_lastCount;
        long int m_currTime;
        long int m_lastReadTime;
        
        int m_pin1, m_pin2;

        Encoder m_encoder;
};