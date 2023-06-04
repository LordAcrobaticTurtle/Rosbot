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

void Rosbot::tankDrive() 
{
    // Update command values
    m_rx.update();
    // m_imu.update();
    // Grab current values + endpoints
    uint16_t channels[TX_NUM_CHANNELS];
    m_rx.getChannels(channels);
    uint16_t channelMinEndpoints[TX_NUM_CHANNELS];
    uint16_t channelMaxEndpoints[TX_NUM_CHANNELS];
    m_rx.getChannelEndpoints(channelMinEndpoints, channelMaxEndpoints);

    uint16_t rollMax, rollMin;
    uint16_t throttleMax, throttleMin;

    float roll = channels[CHANNEL_ROLL];
    float throttle = channels[CHANNEL_THROTTLE];

    throttleMax = channelMaxEndpoints[CHANNEL_THROTTLE];
    throttleMin = channelMinEndpoints[CHANNEL_THROTTLE];
    rollMax = channelMaxEndpoints[CHANNEL_ROLL];
    rollMin = channelMinEndpoints[CHANNEL_ROLL];
}

void Rosbot::update() {
    m_tf = millis();
    float dt = m_tf - m_ti;
    m_imu.update(dt/1000.0);
    m_rx.update();
    // m_encoderL.update();
    // m_encoderR.update();
    double scaleFactor = 1.0;
    m_rx.getChannelPercentage(m_channels, scaleFactor);
    double steering = m_channels[0];
    double throttle = m_channels[2];
    // steering = floatMap(steering, 0, scaleFactor, -1, 1)*255.0;

    if (!m_rx.isSafetyOff() || m_rx.hasLostConnection()) {
        m_driverL.setThrottle(0);
        m_driverR.setThrottle(0);
    }



    if (millis() - m_timer > 100) {
        m_timer = millis();
        Serial.println("TX: " + String(throttle) + ", " + String(steering));
        // m_status.toggleGreen();
    }

    m_ti = m_tf;
}

void Rosbot::printRobotState() {

}