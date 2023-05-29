#include <drivers/N20Encoder.h>

N20Encoder::N20Encoder(uint32_t pinC1, uint32_t pinC2) {

    m_pinC1 = pinC1;
    m_pinC2 = pinC2;
}

void N20Encoder::setup() {
    pinMode(m_pinC1, INPUT_PULLUP);
    pinMode(m_pinC2, INPUT_PULLUP);
}

void N20Encoder::update() {
    
}


