#include <comms/BleTransceiver.h>


BLETransceiver::BLETransceiver(HardwareSerial *hwSerial) {
    m_hwSerial = hwSerial;
}