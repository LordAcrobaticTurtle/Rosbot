#pragma once
#include <Arduino.h>
#include <i2c_device.h>
#include <i2c_driver_wire.h>

#define INT_PIN 2

class IMU {
    public:

    private:
        I2CDriverWire * m_wireObj;        
        bool blinkState = false;
                
        // MPU control/status vars
        bool dmpReady = false;  // set true if DMP init was successful
        uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        uint16_t fifoCount;     // count of all bytes currently in FIFO
        uint8_t fifoBuffer[64]; // FIFO storage buffer

        // // orientation/motion vars
        // Quaternion q;           // [w, x, y, z]         quaternion container
        // VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        // VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        // VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        // VectorFloat gravity;    // [x, y, z]            gravity vector
        float euler[3];         // [psi, theta, phi]    Euler angle container
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

};