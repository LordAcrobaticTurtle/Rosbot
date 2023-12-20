#pragma once
#include <memory>
#include <utility/utility.h>
#include <interfaces/encoder_interface.h>
#include <interfaces/imu_interface.h>
#include <utility/math.h>

class Localisation {
public:
    Localisation(
        std::shared_ptr<ImuInterface> imu,
        std::shared_ptr<EncoderInterface> encoderL,
        std::shared_ptr<EncoderInterface> encoderR
    );

    void resetImu();

    vector3D getAccel();
    vector3D getOrientation();
    vector3D getAngularRates();
    vector2D getWheelVelocity();
    vector3D getPosition(); 
    void run();
    
protected:

    std::shared_ptr<ImuInterface> m_imu;
    std::shared_ptr<EncoderInterface> m_encoderL;
    std::shared_ptr<EncoderInterface> m_encoderR;
    
    // High level variables
    vector3D m_orientation;
    vector3D m_position;
    vector3D m_gyroRates;
    vector3D m_accelReadings;
};




class RobotModel {
    public:
        RobotModel();
        void updateModel();

    private:
        Matrix dx; // derived state
        Matrix x;  // State
        Matrix u;  // input
        Matrix A;  // System
        Matrix B;  // oinput
        Matrix C;  // Output
        Matrix D;  // Output


    
};