#pragma once
#ifdef __USE_I2C_DRIVER_WIRE__
#include <Arduino.h>
#include <i2c_device.h>
#include <memory>
//  Functionality
//  ====================================================================
    // I2C functionality. The address pointer on the AS5600 is automatically incremented after reach read. 
    // Except on the angle, raw and and magnitude registers
    // If the address ptr is set to the high byte of these registers, the ptr is not incremented, 
    // so a re-read of these registers doesn't require an i2c write command

    // Check connection exists
    // -> HANDSHAKE
    // Setup 
    // -> Change units
    // -> Adjust data rate
    // -> Adjust output method
    // -> Check status register to confirm magnet is in right position. 

    // Request data
    //  -> Begin transmission to address
    //  -> Write bytes to address
    //  -> end transmission

    // Receive data  
    // -> Check if data is available
    // -> Write it into some sort of buffer
    // -> Might have to change depending on what type of data it is

    // Adjust settings
    // -> E.g. Step response + filter settings
    // -> Fast filter can be switched on by setting FTH bits in CONF register
    // -> HYsteresis can be enabled to avoid the output changing while not moving. 

#define AS5600_ADDRESS 0x36

class AS5600 {
    public:

        AS5600();
        ~AS5600();
    //  ====================================================================
        
        // Returns an int that indicates the status of the AS5600
        // 0 = Connected and transmitting data
        // 1 = ERROR
        int setup(I2CMaster &interface);
        // // Number of bytes requested
        // int requestData(unsigned int p_numBytesRequested, unsigned char * p_buffer, unsigned int p_bufferSize);
        // int writeData(unsigned char * p_buffer, unsigned int p_bufferSize);
        double getAngle();
        unsigned char getStatus();
        unsigned char getAGC();
        double getVelocity();
    
    private:
        // I2CDriverWire *m_wireObj;
        const int m_ADDRESS = 0x36;
        std::shared_ptr<I2CDevice> m_interface;

        unsigned int m_angleRaw;
        unsigned int m_prevAngleRaw;
        unsigned int m_angleLarge;
        unsigned int m_prevAngleLarge;

        double m_angle;
        double m_prevAngle;

        long int m_currTime;
        long int m_prevTime;
        double m_currVelocity;

        const int m_aoutPin;
        const int m_dirPin ;
        const int m_GPOPin ;

        bool m_isConnected;

        unsigned char m_status;
        unsigned char m_AGC;
        
        enum registerMap {
          ZMCO, 
          ZPOSL, ZPOSH,
          MPOSL, MPOSH,
          MANGL, MANGH,
          CONFL, CONFH,
          RAWANGLEL, RAWANGLEH,
          ANGLEL, ANGLEH,
          STATUS,
          AGC,
          MAGNITUDEL, MAGNITUDEH
        };

        char registerAddresses[17] = {
            // Configuration registers
            // ZMCO
            0x00, 
            // ZPOS
            0x01, 0x02, 
            // MPOS
            0x03, 0x04, 
            // MANG
            0x05, 0x06, 
            // CONF
            0x07, 0x08,
            // Output registers
            // Raw angle
            0x0C,0x0D,
            // Angle
            0x0E,0x0F,
            // Status registers
            // STATUS
            0x0B,
            // AGC
            0x1A,
            // MAGNITUDE
            0x1B,0x1C
        };
};
#endif