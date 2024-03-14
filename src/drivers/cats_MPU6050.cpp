#include <drivers/cats_mpu6050.h>

#define GRAVITY 9.81

catsMPU6050::catsMPU6050() {
    m_imu.initialize();
    pinMode(m_interruptPin, INPUT);
    Wire.begin();
    Wire.setClock(400000);
    
    if (m_imu.dmpInitialize() == 0) {
        m_imu.CalibrateAccel(6);
        m_imu.CalibrateGyro(6);
        m_imu.setDMPEnabled(true);
        m_dmpReady = true;
        // attachInterrupt(m_interruptPin, dmpDataReady, RISING);
        m_imu.setFullScaleAccelRange(3);
        m_imu.setFullScaleGyroRange(3);
    }

}


int catsMPU6050::run () {
    if (!m_dmpReady) {
        Serial.println("DMP not ready");
        return 0;
    }

    if (m_imu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        VectorInt16 accelRaw;
        VectorInt16 gyroRaw;
        m_imu.dmpGetQuaternion(&q, fifoBuffer);
        m_imu.dmpGetGravity(&gravity, &q);
        m_imu.dmpGetEuler(ypr, &q);
        m_imu.dmpGetAccel(&accelRaw, fifoBuffer);
        m_imu.dmpGetGyro(&gyroRaw, fifoBuffer);
        axf = accelRaw.x / 2048.0 * GRAVITY;
        ayf = accelRaw.y / 2048.0 * GRAVITY;
        azf = accelRaw.z / 2048.0 * GRAVITY;

        gxf = gyroRaw.x / 16.4;
        gyf = gyroRaw.y / 16.4;
        gzf = gyroRaw.z / 16.4;
    } else {
        Serial.println("DMP not ready");
    }

    return 0;
}


int catsMPU6050::readImuData (ImuData &data) {
    readAccel(data.accelData);
    readGyroRates(data.gyroRates);
    readOrientation(data.orientation);
    return 0;
}

int catsMPU6050::readAccel (vector3D &data) {
    data.x = axf;
    data.y = ayf;
    data.z = azf;

    return 0;
}

int catsMPU6050::readGyroRates (vector3D &data) {
    data.x = gxf;
    data.y = gyf;
    data.z = gzf;

    return 0;
}

int catsMPU6050::readOrientation (vector3D &data) {
    data.x = ypr[2] * 180 / M_PI;
    data.y = ypr[1] * 180 / M_PI;
    data.z = ypr[0] * 180 / M_PI;

    return 0;
}

