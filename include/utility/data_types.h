#pragma once
#include "utility/math.h"
#include <stdio.h>

typedef unsigned char byte;


struct vector3D {
    float x, y, z;

    vector3D () {
        x = 0;
        y = 0;
        z = 0;
    }
    /**
     * @brief Stores the vector3D contents into a string. Expects a buffer size of at least 64 bytes
     * @param buffer 
     */
    void toString(char *buffer) {
        sprintf(buffer, "%f,%f,%f", x, y, z);
    }

    void add (float ox, float oy, float oz) {
        x += ox;
        y += oy;
        z += oz;
    }

    void subtract (float ox, float oy, float oz) {
        x -= ox;
        y -= oy;
        z -= oz;
    }

    void reset () {
        x = 0;
        y = 0;
        z = 0;
    }
};

struct vector2D {
    float v1, v2;

    void toString(char *buffer) {
        sprintf(buffer, "%f,%f", v1, v2);
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