#pragma once
#ifdef __USE_I2C_DRIVER_WIRE__
#include <i2c_device.h>
#include <i2c_driver_wire.h>



class I2Cinterface {
    public:

        enum I2Ccodes {
            SUCCESS = 0, 
            MSG_TOO_LONG, 
            NACK_ADDRESS,
            NACK_DATA,
            OTHER,
            TIMEOUT
        };

        I2Cinterface();

        void setup(I2CDriverWire * wirePtr, int deviceAddress);
        
        int i2cWrite(byte * buffer, int bufferLength);
        int i2cRead(byte * outBuffer, int bufferLength);
        
        bool isDeviceConnected();
    private:


    private:
        I2CDriverWire * m_wirePtr;
        int m_deviceAddress;

};
#endif