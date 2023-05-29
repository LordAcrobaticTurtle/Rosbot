#include "drivers/IMU.h"

IMU::IMU(I2CMaster *interface)
{
    
}

IMU::~IMU() {}

void IMU::setup(I2CMaster *interface) {
    
}

void IMU::update() {
    // 
    getRawSensorRegisters();
    parseRawData();
}
 
void IMU::getRawSensorRegisters() {
    byte value = registerMap::ACCEL_XOUT_H;
 
}

void IMU::parseRawData() {
    // Take the m_rawRegisters and put 6 bytes into each 

    // Accel is first in buffer
    
    // m_accelData[i] = m_rawRegisters[i] << 8 | m_rawRegisters[i+1];
    m_accelData[0] = m_rawRegisters[0] << 8 | m_rawRegisters[1];
    m_accelData[1] = m_rawRegisters[2] << 8 | m_rawRegisters[3];
    m_accelData[2] = m_rawRegisters[4] << 8 | m_rawRegisters[5];

    // Temp
    m_temp = m_rawRegisters[6] << 8 | m_rawRegisters [7];

    m_gyroData[0] = m_rawRegisters[8] << 8 | m_rawRegisters[9];
    m_gyroData[1] = m_rawRegisters[10] << 8 | m_rawRegisters[11];
    m_gyroData[2] = m_rawRegisters[12] << 8 | m_rawRegisters[13];

    for (int i = 0; i < 3; i++) {
        Serial.println(String(m_gyroData[i]) + ", ");
    }

    Serial.println();

}