#include <comms/BleTransceiver.h>


BLETransceiver::BLETransceiver(HardwareSerial *hwSerial) {
    m_hwSerial = hwSerial;
}

int BLETransceiver::readBytes(byte * buffer, uint32_t bufferLength) {
    
    if (m_hwSerial == NULL) {return -1;}

    if (m_hwSerial->available()) {
        m_hwSerial->readBytes(buffer, bufferLength);
    }

    return 0;
}

int BLETransceiver::sendBytes(byte *buffer, uint32_t bufferLength) {
    if (buffer == NULL) return EARG;
    if (bufferLength <= 0) return EARG;

    if (m_hwSerial->availableForWrite() >= bufferLength) {
        int error = m_hwSerial->write(buffer, bufferLength);
    } else {
        return ENOSPC;
    }

    return 0;
}