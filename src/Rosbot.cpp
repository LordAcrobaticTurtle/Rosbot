#include <Rosbot.h>
#include <drivers/bluetooth_transceiver.h>
// #include <drivers/mpu6050.h>
#include <drivers/DRV8876.h>
#include <drivers/encoder_N20.h>
#include <utility/timing.h>
#include <drivers/cats_mpu6050.h>
#include <hardwareParameters/hardwareParameters.h>
#include <elapsedMillis.h>

Rosbot::Rosbot() : 
    m_status(4,3,2,false),
    m_isStandbyOn(true),
    m_isControlOn(false),
    m_isLocalisationOn(false),
    m_isRadioConnected(false),
    m_A(4,4),
    m_B(4,1),
    m_C(1,4),
    m_D(1,1),
    m_state(4,1),
    m_desiredState(4,1),
    m_dstate(4,1),
    m_K(1,4)
{ 
    m_anglePidParams.bounds[0] = -1;
    m_anglePidParams.bounds[1] = 1;
    m_anglePidParams.kp = 0.1;
    m_anglePidParams.ki = 0.001;
    m_anglePidParams.kd = 0.001;
    
    m_positionPidParams.bounds[0] = -1;
    m_positionPidParams.bounds[1] = 1;
    m_positionPidParams.dt = 0.01;
    
    m_motorLPositionParams.bounds[0] = -1;
    m_motorLPositionParams.bounds[1] = 1;
    m_motorLPositionParams.dt = 0.01;

    m_motorRPositionParams.bounds[0] = -1;
    m_motorRPositionParams.bounds[1] = 1;
    m_motorRPositionParams.dt = 0.01;

    m_qEst.q1 = 1;
    m_qEst.q2 = 0;
    m_qEst.q3 = 0;
    m_qEst.q4 = 0;

    // Initialisation of matrix params.
    // assuming state is x, dx, theta, dtheta    
    m_B.data[0][0] = 0;
    m_B.data[1][0] = 1.0 / HardwareParameters::cartWeight;
    m_B.data[2][0] = 0;
    m_B.data[3][0] = 1.0 / (HardwareParameters::cartWeight * HardwareParameters::pendulumLength);

    m_A.data[0][1] = 1.0;
    m_A.data[1][2] = HardwareParameters::pendulumWeight * HardwareParameters::gravity / HardwareParameters::cartWeight;
    m_A.data[2][3] = 1.0;
    m_A.data[3][2] = (HardwareParameters::cartWeight + HardwareParameters::pendulumWeight) * HardwareParameters::gravity /
                     (HardwareParameters::cartWeight * HardwareParameters::pendulumLength);

    m_K.data[0][0] = -1.0;
    m_K.data[0][1] = -1.1116;
    m_K.data[0][2] = 7.7973;
    m_K.data[0][3] = 0.9166;
    
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    // m_imu.setup(Master);
    // Drivers need to inherit
    // Then can create drivers here, and pass into respective classes
    m_status.switchRedOn();
    
    m_imu = std::make_shared<catsMPU6050>();
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
    m_anglePidParams.errorSum = 0;
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
    m_status.switchRedOn();
    m_encoderL->reset();
    m_encoderR->reset();
    m_imu->reset();
    m_status.switchGreenOn();
    
}

void Rosbot::setMotorPosition (int motorIndex, int throttle) {
    
    if (motorIndex == 0) {
        m_motorL->setThrottle(throttle);
    } else if (motorIndex == 1) {
        m_motorR->setThrottle(throttle);
    }
}

ControlResponse Rosbot::getControlResponse() {
    ControlResponse res;
    res.params = m_anglePidParams;    
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
    m_rx->run();
    // This can be written more efficiently
    double channels[16];
    memset(channels, 0, sizeof(double)*16);

    // m_rx->getChannelPercentage(channels, 1.0);

    // if (m_rx->isSafetyOff()) {
    //     m_isControlOn = true;
    //     m_isLocalisationOn = true;
    //     m_isStandbyOn = false;
    // } else {
    //     m_isLocalisationOn = false;
    //     m_isControlOn = false;
    //     m_isStandbyOn = true;
    // }


    if (m_isStandbyOn) {
        m_status.mix(255, 163, 0);
        m_motorL->setThrottle(0);
        m_motorR->setThrottle(0);
        return;
    } else if (!m_isStandbyOn) {
        m_status.mix(0, 0, 255);
    }

    
    if (m_isLocalisationOn) {
        static FrequencyTimer funcTimer(HZ_200_MICROSECONDS); 

        if (!funcTimer.checkEnoughTimeHasPassed()) {
            return;
        }

        // Confirm the function doesn't run slower than 0.001s 
        long int start = micros();
        runLocalisation();
        long int end = micros();
        // if (end - start >= 1000) {
        // Serial.println("Localisation run time: " + String(end-start));
        // }
    }



    if (m_isControlOn) {
        // static FrequencyTimer funcTimer(HZ_200_MICROSECONDS);

        // if (!funcTimer.checkEnoughTimeHasPassed()) {
        //     return;
        // }
        long int start = micros();
        runControl();     
        long int end = micros();
        // if (end - start >= 1000) {
        // Serial.println("Control run time: " + String(end-start));

    }
}

void Rosbot::runControl () {
    
        // // Scale to 10 degs
    double channels[16];
    memset(channels, 0, sizeof(double)*16);

    double scaleFactor = 1.0;
    m_rx->getChannelPercentage(channels, scaleFactor);
    // m_anglePidParams.target = channels[CHANNEL_ROLL];
    double steering = channels[0];
    double throttle = channels[1];

    steering = floatMap(steering, 0, scaleFactor, -50.0, 50.0);
    throttle = floatMap(throttle, 0, scaleFactor, -20.0, 20.0);


    // Update state for control
    
    // Calculate meteres from origin point. 
    long int position = m_encoderL->readPosition();
    // Counts /= (7*100) -> wheel Revs from origin * wheel circumference = metres from origin
    float metresFromOrigin = (float) position * 2.0 * PI * HardwareParameters::wheelRadius / (7.0 * 100.0);

    float wheelRotVelocity = m_encoderL->readRPM();
    float translationVelocity = wheelRotVelocity * 60.0/(2*PI) * HardwareParameters::wheelRadius;

    m_state.data[0][0] = metresFromOrigin;
    m_state.data[1][0] = translationVelocity;
    m_state.data[2][0] = m_imuData.orientation.x * PI/ 180.0;
    m_state.data[3][0] = m_imuData.gyroRates.x * PI / 180.0;
    
    m_motorLPositionParams = m_positionPidParams;
    m_motorRPositionParams = m_positionPidParams;

    m_motorLPositionParams.currValue = m_encoderL->readPosition();
    m_motorRPositionParams.currValue = m_encoderR->readPosition();
    
    m_desiredState.data[0][0] = 0;
    m_desiredState.data[1][0] = 0;
    m_desiredState.data[2][0] = 0;
    m_desiredState.data[3][0] = 0;

    Matrix error = Matrix::subtract(m_desiredState, m_state);
    Matrix u = Matrix::multiply(m_K, error);
    Serial.println("x1: " + String(m_state.data[0][0]) + ", x2: " + String(m_state.data[1][0]) + ", x3: " + String(m_state.data[2][0]) + ", x4: " + String(m_state.data[3][0]));
    m_motorL->setThrottle(-u.data[0][0]);
    m_motorR->setThrottle(u.data[0][0]);
    Serial.println(u.data[0][0]);
    // m_motorL->setTorqueTarget(u.data[0][0] * HardwareParameters::wheelRadius);
    // m_motorR->setTorqueTarget(-u.data[0][0] * HardwareParameters::wheelRadius);
    // m_motorL->setTorque(u*HardwareParameters::wheelRadius)
    // m_motorR->setTorque(-u*HardwareParameters::wheelRadius)
    
}

void Rosbot::runLocalisation () {
    // Localisation updates
    // Time since last update

    // Running each sub module responsible for driver level work
    m_imu->run();
    m_encoderL->run();
    m_encoderR->run();
    
    m_imu->readImuData(m_imuData);

    // m_imuData.orientation.scale(180.0 / M_PI, 180.0 / M_PI, 180.0 / M_PI);
    
    m_vwheel.v1 = m_encoderL->readRPM();
    m_vwheel.v2 = m_encoderR->readRPM();

    // Convert encoder position counts to metres travelled
    // Take derivative of encoder counts to get velocity. 

}

void Rosbot::runOffsetEstimation () {
    const int numSamples = 200;
    m_status.mix(255,0,255);
    delay(10);

    m_zeroOffsetData.accelData.reset();
    m_zeroOffsetData.orientation.reset();
    m_zeroOffsetData.gyroRates.reset();

    vector3D angleOffset, gyroRateOffset, accelOffset;

    for (int i = 0; i < numSamples; i++) {
        runLocalisation();

        angleOffset.x += m_imuData.orientation.x;
        angleOffset.y += m_imuData.orientation.y;
        angleOffset.z += m_imuData.orientation.z;

        gyroRateOffset.x += m_imuData.gyroRates.x;
        gyroRateOffset.y += m_imuData.gyroRates.y;
        gyroRateOffset.z += m_imuData.gyroRates.z;

        accelOffset.x += m_imuData.accelData.x;
        accelOffset.y += m_imuData.accelData.y;
        accelOffset.z += m_imuData.accelData.z;
    }

    m_zeroOffsetData.orientation.x = angleOffset.x / numSamples;
    m_zeroOffsetData.orientation.y = angleOffset.y / numSamples;
    m_zeroOffsetData.orientation.z = angleOffset.z / numSamples;

    m_zeroOffsetData.accelData.x = accelOffset.x / numSamples;
    m_zeroOffsetData.accelData.y = accelOffset.y / numSamples;
    m_zeroOffsetData.accelData.z = accelOffset.z / numSamples;

    m_zeroOffsetData.gyroRates.x = gyroRateOffset.x / numSamples;
    m_zeroOffsetData.gyroRates.y = gyroRateOffset.y / numSamples;
    m_zeroOffsetData.gyroRates.z = gyroRateOffset.z / numSamples;
    
    
    m_status.mix(0,255,0);
}

vector3D Rosbot::getAngleOffsets () {
    return m_zeroOffsetData.orientation;
}

PIDParams Rosbot::getAnglePIDParams () {
    return m_anglePidParams;
}

void Rosbot::setAnglePIDParams (PIDParams params) {
    m_anglePidParams = params;
}

void Rosbot::setPositionPIDParams (PIDParams params) {
    m_positionPidParams = params;
}

PIDParams Rosbot::getPositionPIDParams () {
    return m_positionPidParams;
}

void Rosbot::setIsRadioConnected (bool isRadioConnected) {
    m_isRadioConnected = isRadioConnected;
}

VerifiedSensorData Rosbot::sensorVerification (int motorIndex, float throttle, float time) {
    // Set a particular voltage on the motors and wait a certain amount of time. 
    VerifiedSensorData data;
    m_status.mix(255,0,255);

    int intThrottle = int(throttle);

    // m_motorL->setThrottle(intThrottle);
    // m_motorR->setThrottle(intThrottle);
    if (motorIndex == 0) {
        m_motorL->setThrottle(intThrottle);
    } else if (motorIndex == 1) {
        m_motorR->setThrottle(intThrottle);
    }
    float timeInMicroseconds = time * 1000000;
    long int start = micros();
    long int end = start;
    long int timeDelay = timeInMicroseconds; // 10 seconds in microseconds

    // At a particular voltage under no load we should have a constant speed. 
    // There should also be an expected range for the encoders to reach.
    // There should also be a constant current draw. 
    long int counter = 0;
    float leftWheelRPM;
    float rightWheelRPM;
    while (end - start <= timeDelay) {
        runLocalisation();  
        // Capture wheel speeds  
        data.velocityAvgLeft += m_vwheel.v1;
        data.velocityAvgRight += m_vwheel.v2;
        char buffer[256];
        m_vwheel.toString(buffer);
        Serial.println("=== === ===");
        Serial.println(buffer);
        Serial.println("PosL: " + String(m_encoderL->readPosition()) + ", PosR: " + String(m_encoderR->readPosition()));
        Serial.println("=== === ===");
        counter++;
        end = micros();
    }

    data.velocityAvgLeft /= (float) counter;
    data.velocityAvgRight /= (float) counter;
    data.positionLeftRaw = m_encoderL->readPosition();
    data.positionRightRaw = m_encoderR->readPosition();
    float leftPositionMetres = m_encoderL->readPosition() * 2.0 * PI * HardwareParameters::wheelRadius / (7.0 / 4.0 * 100.0);
    float rightPositionMetres = m_encoderR->readPosition() * 2.0 * PI * HardwareParameters::wheelRadius / (7.0 / 4.0 * 100.0);
    Serial.println("=== Sensor Verification Results === ");
    Serial.println("Left wheel travel: " + String(leftPositionMetres));
    Serial.println("Right wheel travel: " + String(rightPositionMetres));
    Serial.println("=== === === === === === === === === ");

    m_motorL->setThrottle(0);
    m_motorR->setThrottle(0);
    resetImu();
    m_status.mix(0,255,0);
    delay(100);
    
    return data;
}

void Rosbot::cascadedControl () {
    
    const float positionFrequency = 0.01; // s
    const float angularFrequency = 0.002; // s
    static FrequencyTimer positionControlTimer(positionFrequency * 100000);
    static FrequencyTimer angularControlTimer(angularFrequency * 100000);
    
    m_positionPidParams.target = 0;
    m_positionPidParams.dt = positionFrequency;
    m_anglePidParams.dt = angularFrequency;
    
    if (positionControlTimer.checkEnoughTimeHasPassed()) {
        m_motorLPositionParams = m_positionPidParams;
        m_motorRPositionParams = m_positionPidParams;
        
        m_motorLPositionParams.currValue = m_encoderL->readPosition();
        m_motorRPositionParams.currValue = m_encoderR->readPosition();

        PIDController::computeResponse(m_motorLPositionParams);
        PIDController::computeResponse(m_motorRPositionParams);
    }
    
    // Feed position response in as target for angle controller. 

    if (angularControlTimer.checkEnoughTimeHasPassed()) {
        m_anglePidParams.target = m_motorLPositionParams.response;
        m_anglePidParams.currValue = m_imuData.orientation.x;
        PIDController::computeResponse(m_anglePidParams);
    }

    m_motorL->setThrottle(int(m_anglePidParams.response * 255.0));
    m_motorR->setThrottle(int(-m_anglePidParams.response * 255.0));
}