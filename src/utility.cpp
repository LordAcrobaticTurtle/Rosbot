#include <utility.h>

int i2c_write(
    I2CDriverWire * p_wirePtr, 
    int p_deviceAddress, 
    unsigned char * p_registers, 
    unsigned int p_length, 
    bool p_stop
) {
    
    // Safety programming
    p_wirePtr->beginTransmission(p_deviceAddress);    
    // unsigned char * ptr = p_registers;
    // ptr != p_registers + p_length
    // ptr++

    // for (unsigned int i = 0; i < p_length; i++) 
    //     p_wirePtr->write(p_registers[i]);
    for (unsigned char * ptr = p_registers; ptr != p_registers + p_length; ptr++) {
        p_wirePtr->write(*ptr);
    }

    // Error checking
    return p_wirePtr->endTransmission(p_stop);
}

int i2c_read(
    I2CDriverWire * p_wirePtr, 
    int p_address, 
    unsigned int p_numBytesRequested, 
    unsigned char * p_buffer, 
    unsigned int p_bufferSize
) {
    // Safe programming
    if (p_bufferSize < p_numBytesRequested) 
        return -1;
    

    // Request the number of bytes from the address
    p_wirePtr->requestFrom(p_address, p_numBytesRequested);
    
    // Copy values into buffer
    unsigned int i = 0;
    while (p_wirePtr->available()) {
        // Write data into the buffer
        p_buffer[i] = p_wirePtr->read();
        i++;
        // Check we are not overflowing the buffer
        if (i > p_bufferSize) {
            memset(p_buffer,-1,p_bufferSize);
            return -1;
        }
    }
    
    return 0;
}