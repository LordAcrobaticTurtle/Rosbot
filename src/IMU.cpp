#include "IMU.h"
#include "utility.h"

IMU::IMU(I2CDriverWire * wireObj) {
    m_wireObj = wireObj;
}
IMU::~IMU() {}


void IMU::update() {
    // 
    getAccel();
}

void IMU::getAccel() {
    // // 3 uint16_t values = 6 bytes
    uint8_t bytes[16];
    unsigned char reg = registerMap::ACCEL_XOUT_H; 
    // Write to register I want to get
    i2c_write(m_wireObj, m_ADDRESS, &reg, 1, true);

    i2c_read(m_wireObj, m_ADDRESS, 6, bytes,6);
    
    for (int i = 0; i < 6 ; i++) {
        Serial.print(bytes[i]);
    }


}
