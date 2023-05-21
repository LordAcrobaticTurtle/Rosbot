#pragma once

#include "drivers/RadioInterface.h"
#include "drivers/RGB_LED.h"
#include "drivers/IMU.h"
#include "drivers/DRV8876.h"
#include "drivers/N20Encoder.h"

class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void tankDrive();
        void update();
    private:
        RadioInterface m_rx;

        DRV8876 m_driverL;
        DRV8876 m_driverR;

        N20Encoder m_encoderL;
        N20Encoder m_encoderR;

        RGBLED m_status;

        long int m_timer;

};
