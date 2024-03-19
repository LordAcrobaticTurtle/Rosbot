// #include <control/control.h>

// #include <Arduino.h>

// Control::Control (
//     std::shared_ptr<Localisation> localisation,
//     std::shared_ptr<DcMotorInterface> motorL,
//     std::shared_ptr<DcMotorInterface> motorR
// ) : m_localisation(localisation),
//     m_motorL(motorL),
//     m_motorR(motorR)
// {
//     m_pidParams.bounds[0] = -1;
//     m_pidParams.bounds[1] = 1;
// }


// int Control::run() {
//     // Get params
//     vector3D orientation = m_localisation->getOrientation();
//     vector3D position = m_localisation->getPosition();
//     // Left is v1, v2 is right
//     vector2D wheelVelocities = m_localisation->getWheelVelocity();

//     // For proof of concept. Angular first
//     m_pidParams.currValue = orientation.x;
    
//     // Perform PID control of angle
//     float response = PIDController::computeResponse(m_pidParams);
//     char buffer[64];
//     orientation.toString(buffer);
//     Serial.println(buffer);
//     // vector3D position = m_localisation->getPosition();
//     // // Left is v1, v2 is right
//     // vector2D wheelVelocities = m_localisation->getWheelVelocity();

//     // // For proof of concept. Angular first
//     // PIDParams params;
    
//     // Response should be limited between 0 and 1.
//     // m_motorL->setVoltage();
    

// }

// PIDParams Control::getParams() {
//     return m_pidParams;
// }

// /**
//  * @brief Function used to control the torque output of the motors
// */
// void Control::torqueControl(std::shared_ptr<DcMotorInterface> motor) {
//     // Calculate the change in the system 
//     // 
//     // float dx[2];
//     // float J = 1;
//     // float b = 1;
//     // float K = 1;
//     // float R = 1;
//     // float L = 1;

//     // I could pass the xdot[1] from the main state estimation as a command to the motors
//     // to rotor 

//     // dx[0] = 
// }

