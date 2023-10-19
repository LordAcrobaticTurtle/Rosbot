#pragma once

#include "drivers/RadioInterface.h"
#include "drivers/RGB_LED.h"
#include "drivers/IMU.h"
#include "drivers/DRV8876.h"
#include <drivers/N20Encoder.h>
// #include "drivers/BleTransceiver.h"
#include <control/pid_controller.h>


typedef struct robotState {
    float x;
    float dx;
    float psi;
    float dpsi;
} robotState;

class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void tankDrive();
        void update();
        
        void test();

    private:
        void printRobotState();
        void motorControl();
    
    private:

        double m_channels[TX_NUM_CHANNELS];
        RadioInterface m_rx;
    
        DRV8876 m_driverL;
        DRV8876 m_driverR;
    
        RGBLED m_status;
    
        IMU m_imu;

        N20Encoder m_encoder1;
        N20Encoder m_encoder2;

        // Modelling
        robotState m_state;

        // Control parameters
        PIDParams m_velocityControl;
        PIDParams m_angleControl;
        PIDParams m_angleRateControl;                
        // PIDParams m_imuParams;   
    
        // BLETransceiver m_bleComms;


        float m_tf;
        float m_ti;
        long int m_timer;

};
