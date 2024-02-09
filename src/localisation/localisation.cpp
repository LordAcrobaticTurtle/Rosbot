#include <localisation/localisation.h>
#include <Arduino.h>

Localisation::Localisation(
    std::shared_ptr<ImuInterface> imu,
    std::shared_ptr<EncoderInterface> encoderL,
    std::shared_ptr<EncoderInterface> encoderR
) : m_imu(imu), 
    m_encoderL(encoderL),
    m_encoderR(encoderR)
{
    // m_qEst = {1, 0, 0, 0};
    m_qEst.q1 = 1;
    m_qEst.q2 = 0;
    m_qEst.q3 = 0;
    m_qEst.q4 = 0;

}


void Localisation::run() {
    static float lastDt = 0;
    // Localisation updates
    // Time since last update
    long int timeInMilliseconds = millis();
    static long int lastTimeInMilliseconds = 0;
    float timeInSeconds = millis() / 1000.0;
    if (timeInMilliseconds - lastTimeInMilliseconds < 10) {
        return;
    }

    m_imu->run();
    ImuData data;
    m_imu->readImuData(data);
    // m_orientation = data.orientation;
    m_accelReadings = data.accelData;
    m_gyroRates = data.gyroRates;

    imu_filter(data.accelData.x, data.accelData.y, data.accelData.z, 
                data.gyroRates.x, data.gyroRates.y, data.gyroRates.z, m_qEst);
    
    float roll, pitch, yaw;
    eulerAngles(m_qEst, &roll, &pitch, &yaw);
    m_orientation.x = roll;
    m_orientation.y = pitch;
    m_orientation.z = yaw;

    char buffer[256];
    m_orientation.toString(buffer);
    Serial.println(buffer);

    m_vwheel.v1 = m_encoderL->readRPM();
    m_vwheel.v2 = m_encoderR->readRPM();
    
    lastDt = timeInSeconds;
    lastTimeInMilliseconds = timeInMilliseconds;
}

void Localisation::resetImu() {
    
}

vector2D Localisation::getWheelVelocity() {
    return m_vwheel;
}

vector3D Localisation::getPosition() {
    vector3D vec;
    vec.x = -1;
    vec.y = -1;
    vec.z = -1;
    return vec;
}

vector3D Localisation::getOrientation() {
    return m_orientation;
}

vector3D Localisation::getAccel() {
    return m_accelReadings;
}

vector3D Localisation::getAngularRates() {
    return m_gyroRates;
}



RobotModel::RobotModel(): 
    dx(4,1),
    x(4,1),
    u(4,1),
    A(4,4),
    B(4,1),
    C(1,4),
    D(0,0)
{
    // Setup A, B, C here

}


void RobotModel::updateModel() {
    
}