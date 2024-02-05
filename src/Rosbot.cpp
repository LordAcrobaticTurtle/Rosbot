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
    m_isLocalisationOn(false),
    m_isRadioConnected(false)
{ 
    m_pidParams.bounds[0] = -1;
    m_pidParams.bounds[1] = 1;

    m_pidParams.kp = 0.1;
    m_pidParams.kd = 0;
    m_pidParams.ki = 0;
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
    m_status.switchRedOn();
    m_imu = std::make_shared<Mpu6050>(Master);
    m_status.switchGreenOn();
    m_motorL = std::make_shared<DRV8876>(12, 11, 10, -1, 5);
    m_motorR = std::make_shared<DRV8876>(23, 21, 14, -1, 20);
    m_encoderL = std::make_shared<EncoderN20>(6,7);
    m_encoderR = std::make_shared<EncoderN20>(8,9);
    m_rx = std::make_shared<RadioInterface>(&Serial1);
    m_rx->setup();
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
    // Reset and recompute angle offsets. 
    runAngleOffsetEstimation();
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

    // This can be written more efficiently
    if (m_isStandbyOn) {
        m_status.mix(255, 163, 0);
        m_motorL->setThrottle(0);
        m_motorR->setThrottle(0);
        return;
    } else if (!m_isStandbyOn) {
        m_status.mix(0, 0, 255);
    }

    // if (m_isRadioConnected && m_rx->hasLostConnection()) {
    //     return;
    // }

    if (m_isLocalisationOn) {
        runLocalisation();
    }

    if (m_isControlOn) {
        runControl();     
    }
}

void Rosbot::runControl () {
        static FrequencyTimer funcTimer(HZ_100_MICROSECONDS);

        if (!funcTimer.checkEnoughTimeHasPassed()) {
            return;
        }

        m_pidParams.currValue = m_imuData.orientation.y;
        m_pidParams.dt = HZ_100_MICROSECONDS;
        m_pidParams.target = 0; // Assume 
        
        // Perform PID control of angle
        float response = PIDController::computeResponse(m_pidParams);

        int PWMresponse = response * 255.0;
        Serial.println(PWMresponse);
        m_motorL->setThrottle(-PWMresponse);
        m_motorR->setThrottle(PWMresponse);
        // Update motors with voltage command

        // char buffer[64];
        // m_imuData.orientation.toString(buffer);
        // Serial.println(buffer);       
}

void Rosbot::runLocalisation () {
    // Localisation updates
    // Time since last update
    static FrequencyTimer funcTimer(HZ_100_MICROSECONDS); 

    if (!funcTimer.checkEnoughTimeHasPassed()) {
        return;
    }

    m_imu->run();
    m_imu->readImuData(m_imuData);
    
    imu_filter(m_imuData.accelData.x, m_imuData.accelData.y, m_imuData.accelData.z, 
                m_imuData.gyroRates.x, m_imuData.gyroRates.y, m_imuData.gyroRates.z, m_qEst);
    
    float roll, pitch, yaw;
    eulerAngles(m_qEst, &roll, &pitch, &yaw);
    // const double pitchOffset = -1.692832802;
    m_imuData.orientation.x = roll - m_angleOffsets.x;
    m_imuData.orientation.y = pitch - m_angleOffsets.y;
    m_imuData.orientation.z = yaw - m_angleOffsets.z;

    // char buffer[256];
    // m_imuData.orientation.toString(buffer);
    // Serial.println(buffer);

    float v1 = m_encoderL->readRPM();
    float v2 = m_encoderR->readRPM();
    
    if (v1 != -1) {
        m_vwheel.v1 = v1;
    }

    if (v2!= -1) {
        m_vwheel.v2 = v2;
    }
}

void Rosbot::runAngleOffsetEstimation () {
    const int numSamples = 5000;
    m_status.mix(255,0,255);
    delay(100);
    vector3D offset;
    m_angleOffsets.x = 0;
    m_angleOffsets.y = 0;
    m_angleOffsets.z = 0;
    for (int i = 0; i < numSamples; i++) {
        runLocalisation();
        offset.x += m_imuData.orientation.x;
        offset.y += m_imuData.orientation.y;
        offset.z += m_imuData.orientation.z;
    }

    m_angleOffsets.x = offset.x / numSamples;
    m_angleOffsets.y = offset.y / numSamples;
    m_angleOffsets.z = offset.z / numSamples;
    m_status.mix(0,255,0);
}



vector3D Rosbot::getAngleOffsets () {
    return m_angleOffsets;
}

PIDParams Rosbot::getPIDParams () {
    return m_pidParams;
}

void Rosbot::setPIDParams (PIDParams params) {
    m_pidParams = params;
}

void Rosbot::setIsRadioConnected (bool isRadioConnected) {
    m_isRadioConnected = isRadioConnected;
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