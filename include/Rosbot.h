#pragma once

#include "AccelStepper.h"

#include "drivers/AS5600.h"
#include "drivers/RadioInterface.h"
#include "SBUS.h"
#include "drivers/IMU.h"

#define MOTORINTERFACETYPE 1

class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();

        void tankDrive();
        
    private:
        RadioInterface m_rx;
        AS5600 m_encL;
        AS5600 m_encR;
        IMU m_imu;

};
