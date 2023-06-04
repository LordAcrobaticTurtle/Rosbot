#pragma once
#include <Arduino.h>
#include <i2c_device.h>
#include <memory>

#define INT_PIN 2
#define MPU6050_ADDRESS 0x68
#define GRAVITY 9.81
// The IMU starts in sleep mode

class IMU {
    public:
        IMU();
        ~IMU();

        // Setup - Take the MPU6050 out of sleep mode
        void setup(I2CMaster &interface);

        // Updates all data objects
        void update(float ts);

    private:
        bool init(I2CMaster &interface);
        void getRawSensorRegisters();
        void parseRawData(); 
        int calculateEulerAngles();

    private:
        double m_dt;
    
        std::shared_ptr<I2CDevice> m_interface;
        bool m_configured;

        byte m_rawRegisters[14];
        // Raw data array
        // [x ,y, z]
        int16_t m_accelData[3];
        int16_t m_temp;
        int16_t m_gyroDataRaw[3];
        //                            Units
        float m_gyroDataF[3];    //   degrees/second
        float m_accelDataF[3];   //   m/s/s
        float m_eulerXYZ[3];     //   radians
        float m_gyroAngle[3];    //   Dead reckoning estimate. degrees
        float m_gyroRateOffset[3];//  degrees/second

        enum registerMap {
            // Self test registers
            SELF_TEST_x = 0x0D,
            SELF_TEST_Y,
            SELF_TEST_Z,
            SELF_TEST_A,
            
            SMPLRT_DIV = 0x19,
            CONFIG,
            GYRO_CONFIG,
            ACCEL_CONFIG, 

            FIFO_EN = 0x23,
            
            INT_PIN_CFG = 0x37,
            INT_ENABLE = 0x38,
            INT_STATUS = 0x3A,

            // Sensor registers
            ACCEL_XOUT_H = 0x3B,
            ACCEL_XOUT_L,
            ACCEL_YOUT_H,
            ACCEL_YOUT_L,
            ACCEL_ZOUT_H, 
            ACCEL_ZOUT_L,
            TEMP_OUT_H,
            TEMP_OUT_L,
            GYRO_XOUT_H,
            GYRO_XOUT_L,
            GYRO_YOUT_H,
            GYRO_YOUT_L,
            GYRO_ZOUT_H,
            GYRO_ZOUT_L,

            USER_CTRL = 0x6A,
            PWR_MGMT_1,
            PWR_MGMT_2,
            FIFO_COUNTH,
            FIFO_COUNTL,
            FIFO_R_W
            
        };

};


        // MPU control/status vars
        // bool dmpReady = false;  // set true if DMP init was successful
        // uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        // uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        // uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        // uint16_t fifoCount;     // count of all bytes currently in FIFO
        // uint8_t fifoBuffer[64]; // FIFO storage buffer

        // // orientation/motion vars
        // Quaternion q;           // [w, x, y, z]         quaternion container
        // VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        // VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        // VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        // VectorFloat gravity;    // [x, y, z]            gravity vector
        // float euler[3];         // [psi, theta, phi]    Euler angle container
        // float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
