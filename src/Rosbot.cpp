#include <Rosbot.h>
#include <drivers/bluetooth_transceiver.h>
#include <drivers/mpu6050.h>
#include <drivers/DRV8876.h>
#include <drivers/encoder_N20.h>


I2CMaster& master = Master;

Rosbot::Rosbot() : 
    m_status(4,3,2,false),
    m_isStandbyOn(true),
    m_isControlOn(false),
    m_isLocalisationOn(false)
{ 
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_status.switchRedOn();
    // m_imu.setup(Master);
    // Drivers need to inherit
    // Then can create drivers here, and pass into respective classes
    std::shared_ptr<Mpu6050> imu = std::make_shared<Mpu6050>(Master);
    m_status.switchGreenOn();
    std::shared_ptr<DRV8876> motorL = std::make_shared<DRV8876>(12, 11, 10, -1, 5);
    std::shared_ptr<DRV8876> motorR = std::make_shared<DRV8876>(23, 21, 14, -1, 20);
    std::shared_ptr<EncoderN20> encoderL = std::make_shared<EncoderN20>(6,7);
    std::shared_ptr<EncoderN20> encoderR = std::make_shared<EncoderN20>(8,9);
    
    m_localisation = std::make_shared<Localisation>(
        imu, encoderL, encoderR
    );

    m_control = std::make_shared<Control>(
        m_localisation, motorL, motorR
    );
}

void Rosbot::ActivateStandbyMode() {
    m_isStandbyOn = true;
    toggleControl(false);
    toggleLocalisation(false);
}

void Rosbot::ActivateCalibration() {
    m_isStandbyOn = false;
    toggleLocalisation(true);
    toggleControl(false);
}

void Rosbot::ActivateControlMode() {
    m_isStandbyOn = false;
    toggleLocalisation(true);
    toggleControl(true);
}

void Rosbot::toggleControl(bool isControlOn) {
    m_isControlOn = !m_isControlOn;
}

void Rosbot::toggleLocalisation(bool isLocalisationOn) {
    m_isLocalisationOn = !m_isLocalisationOn;
}

void Rosbot::resetImu() {
    m_localisation->resetImu();
}

ControlResponse Rosbot::getControlResponse() {
    ControlResponse res;
    res.params = m_control->getParams();    
    res.controlIDPlaceholder = 0;
    res.controlResponse = 0;
    return res;
}

LocalisationResponse Rosbot::getLocalisationResponse() {
    LocalisationResponse res;
    res.accelReadings = m_localisation->getAccel();
    res.encoderVelocities = m_localisation->getWheelVelocity();
    res.gyroRates = m_localisation->getAngularRates();
    res.orientation = m_localisation->getOrientation();
    return res;
}

void Rosbot::run() {
    // If standby is on, comms must still run

    if (m_isStandbyOn) {
        m_status.mix(255, 163, 0);
        return;
    } else if (!m_isStandbyOn) {
        m_status.mix(0, 0, 255);
    }
    
    m_localisation->run();
    m_control->run();
    

    // m_tf = millis();
    // float dt = m_tf - m_ti;
    // m_imu.update(dt/1000.0);
    // m_rx.update();
    // float *eulerXYZ = m_imu.getEulerXYZ();
    
    // double scaleFactor = 1.0;
    // m_rx.getChannelPercentage(m_channels, scaleFactor);
    // double steering = m_channels[0];
    // double throttle = m_channels[2];
    // steering = floatMap(steering, 0, scaleFactor, -0.5, 0.5);
    // throttle = floatMap(throttle, 0, scaleFactor, -1, 1);
    
    // m_angleControl.currValue = eulerXYZ[0];
    // m_angleControl.kp = 1.0; // 3.0
    // m_angleControl.ki = 0.2; //0.1
    // m_angleControl.target = 0.05;
    // m_angleControl.dt = dt;
    
    // // float response = PIDController::computeResponse(m_angleControl);
    // float response = 0;
    // // int responseInt = static_cast<int>(response);

    // int sumL = (throttle + steering + response)*PWM_MAX;
    // int sumR = (throttle - steering + response)*PWM_MAX;

    // if (!m_rx.isSafetyOff() || m_rx.hasLostConnection()) {
    //     m_driverL.setThrottle(0);
    //     m_driverR.setThrottle(0);
    //     m_status.mix(255, 163, 0);
    // } else {
    //     m_driverL.setThrottle(-sumL);
    //     m_driverR.setThrottle(sumR);
    //     m_status.mix(0,255,0);
    // }
 
    // if (millis() - m_timer > 100) {
    //     // 
    //     m_timer = millis();
    //     // Serial.println("Response: " + String(response) + ", Angle: " + String(m_angleControl.currValue));
    // }

    // motorControl();
    
    // m_ti = m_tf;
}

// void Rosbot::printRobotState() {

// }

// void Rosbot::motorControl() {
//     // For both left and right motors
//     // I need to control the current of the motor, then I can do velocity
//     // Current first

//     double currentL = m_driverL.readCurrent();
    


// }

// void Rosbot::test() {
//     m_encoder1.update();
//     // m_encoder2.update();
// }