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

extern volatile int enc1Count;
extern volatile int enc2Count;

void encoder1_c1_callback();
void encoder1_c2_callback();
void encoder2_c1_callback();
void encoder2_c2_callback();

// volatile int prevR = 0;
// volatile int currR = 0;
// volatile bool motordirR;
// volatile float rpmR;

// volatile int prevL = 0;
// volatile int currL = 0;
// volatile bool motordirL;
// volatile float rpmL = 0;

class EncoderN20 : public EncoderInterface {

    public:
        EncoderN20(int pin1, int pin2);
        ~EncoderN20() {};
        virtual float readRPM();

        int setup();

        void update();

    private:
    

    private:
        const int pulsesPerRevolution = 7;
        
        long int m_lastCount;
        long int m_lastReadTime;
        

        Encoder m_encoder;
};