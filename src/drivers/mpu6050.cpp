#include "drivers/mpu6050.h"

#ifdef __USE_I2C_DRIVER_WIRE__

Mpu6050::Mpu6050(I2CMaster &interface)
{
    memset(m_eulerXYZ, 0, sizeof(float) * 3);
    memset(m_gyroAngle, 0, sizeof(float) * 3);
    memset(m_gyroRateOffset, 0, sizeof(float) * 3);
    m_configured = init(interface);

    
    // setup();
}

int Mpu6050::readImuData (ImuData &data) {
    readAccel(data.accelData);
    readGyroRates(data.gyroRates);
    readOrientation(data.orientation);
    readMagnetField(data.magneticField);
}

int Mpu6050::readGyroRates(vector3D &rates) {
    rates.x = m_gyroDataF[0];
    rates.y = m_gyroDataF[1];
    rates.z = m_gyroDataF[2];
    return 0;
}

int Mpu6050::readOrientation(vector3D &orientation) {
    orientation.x = m_eulerXYZ[0];
    orientation.y = m_eulerXYZ[1];
    orientation.z = m_eulerXYZ[2];
}

int Mpu6050::readAccel(vector3D &accel) {
    accel.x = m_accelDataF[0];
    accel.y = m_accelDataF[1];
    accel.z = m_accelDataF[2];
    return 0;
}

int Mpu6050::readTemperature(float *temp) {
    return -1;
}

int Mpu6050::readMagnetField(vector3D &field) {
    return -1;
}

void Mpu6050::setup() {
    
    // Compute bias of gyroscope at rest
    const int numSamples = 1000;
    float gyroSamples[3] = {0,0,0};

    for (int i = 0; i < numSamples; i++) {
        getRawSensorRegisters();
        parseRawData();
        for (int i = 0; i < 3; i++) {
            gyroSamples[i] += m_gyroDataF[i];
        }
    }

    Serial.print("GyroOffsets: ");
    for (int i = 0; i < 3; i++) {
        m_gyroRateOffset[i] = gyroSamples[i] / numSamples;
    }
    Serial.println();

    m_gyroAngle[0] = -atan2(m_accelDataF[2], m_accelDataF[1]); // Initiate setup for absolute angles 
    
}

bool Mpu6050::init(I2CMaster &interface) {
    m_interface = std::make_shared<I2CDevice>(interface, MPU6050_ADDRESS, __ORDER_BIG_ENDIAN__);
    uint16_t wakeup = 0;
    uint8_t gyroScale = 0b00010000;

    // 8g scale. Needs 2 written to AFS_SEL
    uint8_t accelScale = 0b00010000;
    // 0x6B
    bool result1 = m_interface->write(registerMap::PWR_MGMT_1, wakeup, false);
    bool result2 = m_interface->write(registerMap::GYRO_CONFIG, gyroScale, false);
    bool result3 = m_interface->write(registerMap::ACCEL_CONFIG, accelScale, false);


    // Read back results
    uint8_t r_gyroScale = 0, r_accelScale = 0;
    m_interface->read(registerMap::GYRO_CONFIG, &r_gyroScale, false);
    m_interface->read(registerMap::GYRO_CONFIG, &r_accelScale, true);

    Serial.println("Gyro reg: " + String(r_gyroScale) + ", Accel reg: " + String(r_accelScale));

    if (!(result1 && result2 && result3)) {
        while (1) {
            Serial.println("IMU incorrectly configured");
        }
    }
    
    return result1 && result2 && result3; 
}

int Mpu6050::run () {
    getRawSensorRegisters();
    parseRawData();
    return 0;
}

void Mpu6050::getRawSensorRegisters() {
    byte value = registerMap::ACCEL_XOUT_H;
    
    if (!m_configured) {
        return;
    }

    for (int i = 0; i < 14; i++) {
        m_interface->read(value+i, &m_rawRegisters[i], true);
    }
}

void Mpu6050::parseRawData() {
    // Take the m_rawRegisters and put 6 bytes into each 
    // Accel is first in buffer
    
    if (!m_configured) {
        return;
    }

    m_accelDataRaw[0] = m_rawRegisters[0] << 8 | m_rawRegisters[1];
    m_accelDataRaw[1] = m_rawRegisters[2] << 8 | m_rawRegisters[3];
    m_accelDataRaw[2] = m_rawRegisters[4] << 8 | m_rawRegisters[5];

    // Temp
    m_temp = m_rawRegisters[6] << 8 | m_rawRegisters [7];

    m_gyroDataRaw[0] = m_rawRegisters[8] << 8 | m_rawRegisters[9];
    m_gyroDataRaw[1] = m_rawRegisters[10] << 8 | m_rawRegisters[11];
    m_gyroDataRaw[2] = m_rawRegisters[12] << 8 | m_rawRegisters[13];

    for (int i = 0; i < 3; i++) {
        m_gyroDataF[i] = m_gyroDataRaw[i] / 32.8; // 32.8 for 1000 deg/s
        m_gyroDataF[i] = (m_gyroDataF[i]) * PI/180; // Convert to radians
        m_accelDataF[i] = m_accelDataRaw[i] / 4096.0 * GRAVITY; // 4096.0 for 8g scale. Change to m/s/s
    }

}

int Mpu6050::calculateEulerAngles() {

    if (!m_configured) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        // Remove bias
        // Switch to radians
        m_gyroDataF[i] = (m_gyroDataF[i] - m_gyroRateOffset[i]) * PI/180; 

        // Update dead reckoning estimate
        m_gyroAngle[i] = m_gyroDataF[i]*m_dt + m_gyroAngle[i];
    }

    double accelAngleX = atan2(m_accelDataF[2], m_accelDataF[1]);
    double alpha = 0.96;

    m_eulerXYZ[0] = m_gyroAngle[0]*alpha + accelAngleX*(1.0-alpha);

    // for (int i = 0; i < 3; i++) {
    //     Serial.print(String(m_eulerXYZ[i]) + ", ");
    // }

    // Serial.println();
    return 0;
}

#endif