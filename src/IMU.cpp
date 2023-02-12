#include "IMU.h"

IMU::IMU()
{

}

IMU::~IMU() {}

void IMU::setup(I2CDriverWire * wireObj) {
    // Unsleep IMU
    // Write a 0x00 to 0x6B
    // Load register address then value to write data
    // unsigned char array[2];

    // Write zero to the sleep register to wake up imu
    m_i2cInterface.setup(wireObj, MPU6050_ADDRESS);

    byte wakeUpBuffer[2] = {0x6B, 0};
    m_i2cInterface.i2cWrite(wakeUpBuffer, 2);

    byte sensitivityBuffer[2] = {0x1B, 0b00001000};
    m_i2cInterface.i2cWrite(sensitivityBuffer, 2);
    
}

void IMU::update() {
    // 
    getRawSensorRegisters();
}

void IMU::getRawSensorRegisters() {
    byte value = registerMap::ACCEL_XOUT_H;
    m_i2cInterface.i2cWrite(&value, 1);
    
    byte requestedValues[14];
    m_i2cInterface.i2cRead(requestedValues, 14);

    for (int i = 0; i < 14; i++) {
        Serial.print(String(requestedValues[i]) + ", " );
    }

    Serial.println();
    // gyr_raw[0] /= 65.5;
    // gyr_raw[1] /= 65.5;
    // gyr_raw[2] /= 65.5;
 
}
