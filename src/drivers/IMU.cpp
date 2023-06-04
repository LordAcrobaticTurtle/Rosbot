#include "drivers/IMU.h"

IMU::IMU()
{
    
}

IMU::~IMU() {}

void IMU::setup(I2CMaster &interface) {
    memset(m_eulerXYZ, 0, sizeof(float) * 3);
    memset(m_gyroAngle, 0, sizeof(float) * 3);
    memset(m_gyroRateOffset, 0, sizeof(float) * 3);

    m_configured = init(interface);
    
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
        Serial.print(String(m_gyroRateOffset[i]) + ", ");
    }
    Serial.println();

    m_gyroAngle[0] = -atan2(m_accelDataF[2], m_accelDataF[1]); // Initiate setup for absolute angles 
    
}

bool IMU::init(I2CMaster &interface) {
    m_interface = std::make_shared<I2CDevice>(interface, MPU6050_ADDRESS, __ORDER_BIG_ENDIAN__);
    uint16_t wakeup = 0;
    uint16_t gyroScale = 0b00000000;
    uint16_t accelScale = 0;
    // 0x6B
    bool result1 = m_interface->write(registerMap::PWR_MGMT_1, wakeup, false);
    bool result2 = m_interface->write(registerMap::GYRO_CONFIG, gyroScale, false);
    bool result3 = m_interface->write(registerMap::ACCEL_CONFIG, accelScale, false);

    return result1 && result2 && result3; 
}

void IMU::update(float dt) {
    m_dt = dt;
    getRawSensorRegisters();
    parseRawData();
    calculateEulerAngles();
}
 
void IMU::getRawSensorRegisters() {
    byte value = registerMap::ACCEL_XOUT_H;
    
    if (!m_configured) {
        return;
    }

    for (int i = 0; i < 14; i++) {
        m_interface->read(value+i, &m_rawRegisters[i], true);
    }
}

void IMU::parseRawData() {
    // Take the m_rawRegisters and put 6 bytes into each 
    // Accel is first in buffer
    
    if (!m_configured) {
        return;
    }

    m_accelData[0] = m_rawRegisters[0] << 8 | m_rawRegisters[1];
    m_accelData[1] = m_rawRegisters[2] << 8 | m_rawRegisters[3];
    m_accelData[2] = m_rawRegisters[4] << 8 | m_rawRegisters[5];

    // Temp
    m_temp = m_rawRegisters[6] << 8 | m_rawRegisters [7];

    m_gyroDataRaw[0] = m_rawRegisters[8] << 8 | m_rawRegisters[9];
    m_gyroDataRaw[1] = m_rawRegisters[10] << 8 | m_rawRegisters[11];
    m_gyroDataRaw[2] = m_rawRegisters[12] << 8 | m_rawRegisters[13];

    for (int i = 0; i < 3; i++) {
        m_gyroDataF[i] = m_gyroDataRaw[i] / 131.0;
        m_accelDataF[i] = m_accelData[i] / 16384.0 * GRAVITY;
    }

    // for (int i = 0; i < 3; i++) {
    //     Serial.print(String(m_accelDataF[i]) + ", ");
    // }
    // Serial.println();
}

int IMU::calculateEulerAngles() {
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
}