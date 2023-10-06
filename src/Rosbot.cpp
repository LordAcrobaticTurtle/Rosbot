#include <Rosbot.h>
#include <utility/math.h>
#include <utility/error_codes.h>
#include <comms/packet.h>
#include <comms/packetID.h>


Rosbot::Rosbot() : 
    m_rx(&Serial1), 
    m_driverL(11, 12, 10, -1, 5), 
    m_driverR(22, 20, 14, -1, 23), 
    m_status(4,3,2,false)
{ 
    m_tf = 0;
    m_ti = 0;
    memset(&m_angleControl, 0, sizeof(PIDParams));
    memset(&m_angleRateControl, 0, sizeof(PIDParams));
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_bleComms.init(&Serial4, 9600);
    m_timer = 0;
    m_rx.setup();
    m_status.switchRedOn();
    m_imu.setup(Master);
    m_status.switchGreenOn();
    // m_encoder1.setup(encoder1_c1_callback, encoder1_c2_callback, &enc1Count, enc1_c1, enc1_c2);
    // m_encoder2.setup(encoder2_c1_callback, encoder2_c2_callback, &enc2Count, enc2_c1, enc2_c2);

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
    m_angleControl.kp = 1.0; // 3.0
    m_angleControl.ki = 0.2; //0.1
    m_angleControl.target = 0.05;
    m_angleControl.dt = dt;
    
    // float response = PIDController::computeResponse(m_angleControl);
    float response = 0;
    // int responseInt = static_cast<int>(response);

    int sumL = (throttle + steering + response)*PWM_MAX;
    int sumR = (throttle - steering + response)*PWM_MAX;

    if (!m_rx.isSafetyOff() || m_rx.hasLostConnection()) {
        m_driverL.setThrottle(0);
        m_driverR.setThrottle(0);
        m_status.mix(255, 163, 0);
    } else {
        m_driverL.setThrottle(-sumL);
        m_driverR.setThrottle(sumR);
        m_status.mix(0,255,0);
    }
 
    if (millis() - m_timer > 100) {
        m_timer = millis();
        // Serial.println("Response: " + String(response) + ", Angle: " + String(m_angleControl.currValue));
    }
    

    commsPacket::State state;
    state.current[0] = m_driverL.readCurrent();
    state.current[1] = m_driverR.readCurrent();
    state.eulerXYZ = m_imu.getEulerXYZ();
    state.velocity[0] = 0;
    state.velocity[1] = 0;
    
    Packet packet;
    packet.m_header.m_dataSize = sizeof(state) + sizeof(PacketHeader);
    packet.m_header.m_packetID = PacketID::STATE;
    packet.m_header.m_timestamp = micros();
    packet.m_data = (unsigned char *) &state;
    
    m_bleComms.sendBytes();

    m_ti = m_tf;
}

void Rosbot::printRobotState() {

}

void Rosbot::test() {
    m_encoder1.update();
    // m_encoder2.update();
}