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

}


void Localisation::run() {
    static float lastDt = 0;
    // Localisation updates
    // Time since last update
    float dt = millis() / 1000.0;
    m_imu->update(dt - lastDt);
    m_imu->readOrientation(m_orientation);
    m_imu->readGyroRates(m_gyroRates);
    lastDt = dt;
}

void Localisation::resetImu() {
    
}

vector2D Localisation::getWheelVelocity() {
    vector2D vec;
    vec.v1 = -1;
    vec.v2 = -1;
    return vec;
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