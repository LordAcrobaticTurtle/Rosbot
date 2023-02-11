#pragma once
#include <i2c_device.h>
#include <i2c_driver_wire.h>



class I2Cinterface {
    public:
        I2Cinterface(I2CDriverWire * wirePtr, int deviceAddress);
        
        void i2cWrite(byte * buffer, int bufferLength);
        void i2cRead(byte * outBuffer, int bufferLength);

        
    private:


    private:
        I2CDriverWire * m_wirePtr;
        int m_deviceAddress;

};