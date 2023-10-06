#pragma once

#include "drivers/RadioInterface.h"
#include "drivers/RGB_LED.h"
#include "drivers/IMU.h"
#include "drivers/DRV8876.h"
#include "drivers/BleTransceiver.h"
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

        double m_channels[TX_NUM_CHANNELS];
        RadioInterface m_rx;
    
        DRV8876 m_driverL;
        DRV8876 m_driverR;
    
        RGBLED m_status;
    
        IMU m_imu;
    
        BLETransceiver m_bleComms;

        PIDParams m_imuParams;   

        float m_tf;
        float m_ti;
        long int m_timer;

};
