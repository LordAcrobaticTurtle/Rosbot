#pragma once

#include "drivers/RadioInterface.h"
#include "drivers/RGB_LED.h"
#include "drivers/IMU.h"
#include "drivers/DRV8876.h"
#include <control/pid_controller.h>

class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void tankDrive();
        void update();

    private:
        void printRobotState();
    
    private:
        RadioInterface m_rx;
        double m_channels[TX_NUM_CHANNELS];

        DRV8876 m_driverL;
        DRV8876 m_driverR;

        RGBLED m_status;

        IMU m_imu;
        PIDParams m_imuParams;                

        float m_tf;
        float m_ti;
        long int m_timer;

};
