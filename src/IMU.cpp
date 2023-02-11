#include "IMU.h"
#include "utility.h"

IMU::IMU(I2CDriverWire * wireObj) : 
    m_i2cInterface(wireObj, MPU6050_ADDRESS)
{

}

IMU::~IMU() {}

void IMU::setup() {
    // Unsleep IMU
    // Write a 0x00 to 0x6B
    // Load register address then value to write data
    // unsigned char array[2];

    // Write zero to the sleep register to wake up imu
    byte wakeUpBuffer[2] = {0x6B, 0};
    m_i2cInterface.i2cWrite(wakeUpBuffer, 2);

    byte sensitivityBuffer[2] = {0x1B, 0b00001000};
    m_i2cInterface.i2cWrite(sensitivityBuffer, 2);
    
}

void IMU::update() {
    // 
    getAccel();
}

void IMU::getAccel() {
    // // 3 uint16_t values = 6 bytes
    // uint8_t bytes[16];
    // unsigned char array[1];
    // array[0] = registerMap::GYRO_XOUT_H;
    // Write to register I want to get
    // i2c_write(m_wireObj, m_ADDRESS, &array[0], 1, false);

    // if (i2c_read(m_wireObj, m_ADDRESS, 6, bytes,6) == -1) {
    //     while (true) {
    //         Serial.println("getAccel: Error retrieving i2c");
    //     }
    // }
    
    // for (int i = 0; i < 6 ; i++) {
    //     Serial.print(bytes[i]);
    // // }
    byte value = 0x43;
    m_i2cInterface.i2cWrite(&value, 1);
    
    byte requestedValues[6];
    m_i2cInterface.i2cRead(requestedValues, 6);

    for (int i = 0; i < 6; i++) {
        Serial.print(String(requestedValues[i]) + ", " );
    }
    Serial.println();

    // m_wireObj->beginTransmission(m_ADDRESS);
    // m_wireObj->write(0x43);
    // m_wireObj->endTransmission(false);
    
    // m_wireObj->requestFrom(m_ADDRESS, 6);

    // float gyr_raw[3];
    // gyr_raw[0] = m_wireObj->read() << 8 | m_wireObj->read();
    // gyr_raw[1] = m_wireObj->read() << 8 | m_wireObj->read();
    // gyr_raw[2] = m_wireObj->read() << 8 | m_wireObj->read();

    // gyr_raw[0] /= 65.5;
    // gyr_raw[1] /= 65.5;
    // gyr_raw[2] /= 65.5;
 
    // for (int i = 0; i < 3; i++) {
    //     Serial.println(gyr_raw[i]);
    // }
}
