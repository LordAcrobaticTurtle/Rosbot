#include <localisation/localisation.h>

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
    // Localisation updates
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
    vector3D vec;
    vec.x = -1;
    vec.y = -1;
    vec.z = -1;
    return vec;
}

void RobotModel::updateModel() {

}