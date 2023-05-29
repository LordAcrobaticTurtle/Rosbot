#pragma once
#include <Arduino.h>


#define GEARING 100
#define ENCODERMULT 14

const int enc2_c1 = 8;
const int enc2_c2 = 9;

const int enc1_c1 = 6;
const int enc1_c2 = 7;

// volatile int prevR = 0;
// volatile int currR = 0;
// volatile bool motordirR;
// volatile float rpmR;

// volatile int prevL = 0;
// volatile int currL = 0;
// volatile bool motordirL;
// volatile float rpmL = 0;


class N20Encoder {

    public:
        N20Encoder(uint32_t pinC1, uint32_t pinC2);

        void setup();
        void update();
        
        void c1_callback();

        uint32_t getpinC1() {return m_pinC1;}
        uint32_t getpinC2() {return m_pinC2;}

    private:
    

    private:
        uint32_t m_pinC1;
        uint32_t m_pinC2;
        double m_rpm;
};