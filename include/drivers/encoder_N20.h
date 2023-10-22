#pragma once
#include <Arduino.h>
#include <interfaces/encoder_interface.h>
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
        EncoderN20();

        virtual float readRPM();

        int setup(  void (*c1_callback)(void), void (*c2_callback)(void), 
                    volatile int *count, 
                    uint32_t pinC1, uint32_t pinC2);
        void update();
 
        uint32_t getpinC1() {return m_pinC1;}
        uint32_t getpinC2() {return m_pinC2;}

    private:
    

    private:

        volatile int *m_count;
        volatile int *m_prevCount;

        uint32_t m_pinC1;
        uint32_t m_pinC2;
        double m_rpm;
};