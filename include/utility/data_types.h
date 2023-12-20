#pragma once
#include "utility/math.h"
#include <stdio.h>

typedef unsigned char byte;


struct vector3D {
    float x, y, z;

    void toString(char *buffer) {
        sprintf(buffer, "(%f,%f,%f)", x, y, z);
    }
};

struct vector2D {
    float v1, v2;

    void toString(char *buffer) {
        sprintf(buffer, "(%f,%f)", v1, v2);
    }
};


class autoIncrementingResetInt {
    public:
        autoIncrementingResetInt(int initVal, int maxVal) {
            m_counter = initVal;
            m_initialValue = initVal;
            m_maxValue = maxVal;
        }

        autoIncrementingResetInt& operator++(int) {
            
            autoIncrementingResetInt temp = *this;
            m_counter++;
            if (m_counter >= m_maxValue) {
                m_counter = m_initialValue;
            }
            
            return temp;
        }

        int getCount() {
            return m_counter;
        }

        void setCount(int value) {
            int clampedValue = clamp(value, m_initialValue, m_maxValue);
            m_counter = clampedValue;
        }

    protected:
        int m_counter;
        int m_initialValue;
        int m_maxValue;

};