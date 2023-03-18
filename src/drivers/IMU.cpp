#include "drivers/IMU.h"

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

    byte gyroSensitivityBuffer[2] = {0x1B, 0b00001000};
    m_i2cInterface.i2cWrite(gyroSensitivityBuffer, 2);
    
}

void IMU::update() {
    // 
    getRawSensorRegisters();
    parseRawData();
}
 
void IMU::getRawSensorRegisters() {
    byte value = registerMap::ACCEL_XOUT_H;
    m_i2cInterface.i2cWrite(&value, 1);
    
    m_i2cInterface.i2cRead(m_rawRegisters, 14);


    // for (int i = 0; i < 14; i++) {
    //     Serial.print(String(m_rawRegisters[i]) + ", " );
    // }

    // Serial.println();
    // gyr_raw[0] /= 65.5;
    // gyr_raw[1] /= 65.5;
    // gyr_raw[2] /= 65.5;
 
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