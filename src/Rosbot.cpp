#include <Rosbot.h>
#include <utility/math.h>


Rosbot::Rosbot() : 
    m_rx(&Serial1), 
    m_driverL(11, 12, 10, -1, 5), 
    m_driverR(22, 20, 14, -1, 23), 
    // m_encoderL(enc1_c1, enc1_c2), 
    // m_encoderR(enc2_c1, enc2_c2),
    m_status(3,4,2,true)
{ 
    m_tf = 0;
    m_ti = 0;
    memset(&m_angleControl, 0, sizeof(PIDParams));
    memset(&m_angleRateControl, 0, sizeof(PIDParams));
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_timer = 0;
    // m_encoderL.setup();
    // m_encoderR.setup();
    m_rx.setup();
    m_status.switchRedOn();
    m_imu.setup(Master);
    m_status.switchGreenOn();

    memset(m_channels, 0, TX_NUM_CHANNELS*sizeof(double));
}

void Rosbot::update() {
    m_tf = millis();
    float dt = m_tf - m_ti;
    m_imu.update(dt/1000.0);
    m_rx.update();
    float *eulerXYZ = m_imu.getEulerXYZ();
    
    double scaleFactor = 1.0;
    m_rx.getChannelPercentage(m_channels, scaleFactor);
    double steering = m_channels[0];
    double throttle = m_channels[2];
    steering = floatMap(steering, 0, scaleFactor, -0.5, 0.5);
    throttle = floatMap(throttle, 0, scaleFactor, -1, 1);
    
    m_angleControl.currValue = eulerXYZ[0];
    m_angleControl.kp = 1;
    m_angleControl.target = 0.0;
    m_angleControl.dt = dt;
    
    float response = PIDController::computeResponse(m_angleControl);
 
    int sumL = (throttle + steering)*PWM_MAX;
    int sumR = (throttle - steering)*PWM_MAX;

    if (!m_rx.isSafetyOff() || m_rx.hasLostConnection()) {
        m_driverL.setThrottle(0);
        m_driverR.setThrottle(0);
        m_status.mix(255, 163, 0);
    } else {
        m_driverL.setThrottle(-sumL);
        m_driverR.setThrottle(sumR);
    }

    if (millis() - m_timer > 100) {
        m_timer = millis();
        Serial.println("Response: " + String(response));
    }

    m_ti = m_tf;
}

void Rosbot::printRobotState() {

}