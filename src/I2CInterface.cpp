#include "I2CInterface.h"


I2Cinterface::I2Cinterface(I2CDriverWire * wirePtr, int deviceAddress) {
    m_wirePtr = wirePtr;
    m_deviceAddress = deviceAddress;
}

void I2Cinterface::i2cWrite(byte * buffer, int bufLength) {
    m_wirePtr->beginTransmission(m_deviceAddress);

    for (byte * ptr = buffer; ptr < buffer + (byte) bufLength; ptr++) {
        m_wirePtr->write(*ptr);
    }

    m_wirePtr->endTransmission(true);
}

void I2Cinterface::i2cRead(byte * outBuf, int requestedBytes) {
    m_wirePtr->requestFrom(m_deviceAddress, requestedBytes);
    
    for (byte * ptr = outBuf; ptr < outBuf + (byte) requestedBytes; ptr++) {
        *ptr = m_wirePtr->read();
    }
}


