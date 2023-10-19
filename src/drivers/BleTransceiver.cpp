#include <drivers/BleTransceiver.h>
#include <comms/packet.h>

BLETransceiver::BLETransceiver() {}

void BLETransceiver::init(HardwareSerial *hwSerial, long int baudrate) {
    m_hwSerial = hwSerial;
    m_hwSerial->begin(baudrate);
    m_hwSerial->setTimeout(10);
}




int BLETransceiver::readBytes(byte * buffer, uint32_t bufferLength) {
    
    if (m_hwSerial == NULL) {return -1;}

    if (m_hwSerial->available()) {
        return m_hwSerial->readBytes(buffer, bufferLength);
    }

    return 0;
}

int BLETransceiver::sendBytes(byte *buffer, uint32_t bufferLength) {
    if (buffer == NULL) return EARG;
    if (bufferLength <= 0) return EARG;

    int error = m_hwSerial->write(buffer, bufferLength);
    // if (m_hwSerial->availableForWrite() >= bufferLength) {
    // } else {
    //     return ENOSPC;
    // }

    return 0;
}