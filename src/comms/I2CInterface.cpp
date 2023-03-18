#include "comms/I2CInterface.h"


I2Cinterface::I2Cinterface() {}

void I2Cinterface::setup(I2CDriverWire * wirePtr, int deviceAddress) {
    m_wirePtr = wirePtr;
    m_deviceAddress = deviceAddress;
}

int I2Cinterface::i2cWrite(byte * buffer, int bufLength) {
    m_wirePtr->beginTransmission(m_deviceAddress);

    for (byte * ptr = buffer; ptr < buffer + (byte) bufLength; ptr++) {
        m_wirePtr->write(*ptr);
    }

    m_wirePtr->endTransmission(true);
}

int I2Cinterface::i2cRead(byte * outBuf, int requestedBytes) {
    m_wirePtr->requestFrom(m_deviceAddress, requestedBytes);
    
    for (byte * ptr = outBuf; ptr < outBuf + (byte) requestedBytes; ptr++) {
        *ptr = m_wirePtr->read();
    }
}

bool I2Cinterface::isDeviceConnected() {
    m_wirePtr->beginTransmission(m_deviceAddress);
    return m_wirePtr->endTransmission(true);
}

