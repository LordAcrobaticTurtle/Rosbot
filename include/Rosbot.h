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

        void targetStepperPosL(int target);
        void targetStepperPosR(int target);
        void stepperRun();

        
    private:
        
        AccelStepper m_stepperL;
        AccelStepper m_stepperR;
        RadioInterface m_rx;
        AS5600 m_encL;
        AS5600 m_encR;
        IMU m_imu;

        int m_stepperMaxSpeed;
        int m_stepperMaxAccel;

        const int m_dirPinL = 3;
        const int m_stepPinL = 2;
        const int m_dirPinR = 14;
        const int m_stepPinR = 12;

};
