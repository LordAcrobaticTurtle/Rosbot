#pragma once
#include <i2c_device.h>
#include <i2c_driver_wire.h>
int i2c_write(I2CDriverWire * p_wirePtr, int p_deviceAddress, unsigned char * p_registers, unsigned int p_length, bool p_stop);
int i2c_read (I2CDriverWire * p_wirePtr, int p_address, unsigned int p_numBytesRequested, unsigned char * p_buffer, unsigned int p_bufferSize);