#include <Rosbot.h>
#include <drivers/bluetooth_transceiver.h>
#include <drivers/mpu6050.h>
#include <drivers/DRV8876.h>
#include <drivers/encoder_N20.h>
#include <utility/timing.h>

I2CMaster& master = Master;

Rosbot::Rosbot() : 
    m_status(4,3,2,false),
    m_isStandbyOn(true),
    m_isControlOn(false),
    m_isLocalisationOn(false)
{ 
    m_pidParams.bounds[0] = -1;
    m_pidParams.bounds[1] = 1;
    m_qEst.q1 = 1;
    m_qEst.q2 = 0;
    m_qEst.q3 = 0;
    m_qEst.q4 = 0;
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_status.switchRedOn();
    // m_imu.setup(Master);
    // Drivers need to inherit
    // Then can create drivers here, and pass into respective classes
    std::shared_ptr<Mpu6050> m_imu = std::make_shared<Mpu6050>(Master);
    m_status.switchGreenOn();
    std::shared_ptr<DRV8876> m_motorL = std::make_shared<DRV8876>(12, 11, 10, -1, 5);
    std::shared_ptr<DRV8876> m_motorR = std::make_shared<DRV8876>(23, 21, 14, -1, 20);
    std::shared_ptr<EncoderN20> m_encoderL = std::make_shared<EncoderN20>(6,7);
    std::shared_ptr<EncoderN20> m_encoderR = std::make_shared<EncoderN20>(8,9);
}

void Rosbot::ActivateStandbyMode() {
    m_isStandbyOn = true;
    setControlMode(false);
    setLocalisationMode(false);
}

void Rosbot::ActivateCalibration() {
    m_isStandbyOn = false;
    setLocalisationMode(true);
    setControlMode(false);
}

void Rosbot::ActivateControlMode() {
    m_isStandbyOn = false;
    setLocalisationMode(true);
    setControlMode(true);
}

void Rosbot::setControlMode(bool isControlOn) {
    m_isControlOn = isControlOn;
}

void Rosbot::setLocalisationMode(bool isLocalisationOn) {
    m_isLocalisationOn = isLocalisationOn;
}

void Rosbot::resetImu() {
    
}

ControlResponse Rosbot::getControlResponse() {
    ControlResponse res;
    res.params = m_pidParams;    
    res.controlIDPlaceholder = 0;
    res.controlResponse = 0;
    return res;
}

LocalisationResponse Rosbot::getLocalisationResponse() {
    LocalisationResponse res;
    res.accelReadings = m_imuData.accelData;
    res.encoderVelocities = m_vwheel;
    res.gyroRates =   m_imuData.gyroRates;
    res.orientation = m_imuData.orientation;
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

    if (m_isLocalisationOn) {
        // Run localisation module
        runLocalisation();
    }

    if (m_isControlOn) {
        runControl();     
    }
}

void Rosbot::runControl () {
        m_pidParams.currValue = m_imuData.orientation.y;
        // Perform PID control of angle
        float response = PIDController::computeResponse(m_pidParams);
        char buffer[64];
        m_imuData.orientation.toString(buffer);
        Serial.println(buffer);       
}

void Rosbot::runLocalisation () {
    // Localisation updates
    // Time since last update
    static FrequencyTimer funcTimer(10000); // -> 0.01 s = 100 Hz

    if (!funcTimer.checkEnoughTimeHasPassed()) {
        return;
    }

    m_imu->run();
    m_imu->readImuData(m_imuData);
    // m_orientation = data.orientation;
    
    imu_filter(m_imuData.accelData.x, m_imuData.accelData.y, m_imuData.accelData.z, 
                m_imuData.gyroRates.x, m_imuData.gyroRates.y, m_imuData.gyroRates.z, m_qEst);
    
    float roll, pitch, yaw;
    eulerAngles(m_qEst, &roll, &pitch, &yaw);
    m_imuData.orientation.x = roll;
    m_imuData.orientation.y = pitch;
    m_imuData.orientation.z = yaw;

    char buffer[256];
    m_imuData.orientation.toString(buffer);
    Serial.println(buffer);

    m_vwheel.v1 = m_encoderL->readRPM();
    m_vwheel.v2 = m_encoderR->readRPM();
    
}
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