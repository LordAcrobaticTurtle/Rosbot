#pragma once
#include <memory>
#include <utility/utility.h>
#include <interfaces/encoder_interface.h>
#include <interfaces/imu_interface.h>

class Localisation {
public:
    Localisation(
        std::shared_ptr<ImuInterface> imu,
        std::shared_ptr<EncoderInterface> encoderL,
        std::shared_ptr<EncoderInterface> encoderR
    );

    vector3D getOrientation();
    vector3D getAngularRates();
    vector2D getWheelVelocity();
    vector3D getPosition();
    void run();
    
private:

    std::shared_ptr<ImuInterface> m_imu;
    std::shared_ptr<EncoderInterface> m_encoderL;
    std::shared_ptr<EncoderInterface> m_encoderR;
    
    // High level variables
    vector3D m_orientation;
    vector3D m_position;
    vector3D m_gyroRates;
};