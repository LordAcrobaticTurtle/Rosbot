#include <Rosbot.h>
// #include <utility/math.h>
// #include <utility/error_codes.h>
// #include <comms/packet.h>
// #include <comms/packetID.h>


    // m_driverL(11, 12, 10, -1, 5), 
    // m_driverR(22, 20, 14, -1, 23), 



Rosbot::Rosbot() : 
    m_status(4,3,2,false),
    m_rx(&Serial1)
{ 

}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    // m_bleComms.init(&Serial4, 9600);
    m_rx.setup();
    m_status.switchRedOn();
    // m_imu.setup(Master);
    m_status.switchGreenOn();
    
    m_localisation = std::make_shared<Localisation>(

    );


    m_control = std::make_shared<Control>(
        m_localisation,
        
    );

    
    m_comms = std::make_shared<Comms>(
        m_localisation, 
        m_control,

    );
    


    memset(m_channels, 0, TX_NUM_CHANNELS*sizeof(double));
}

void Rosbot::run() {
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