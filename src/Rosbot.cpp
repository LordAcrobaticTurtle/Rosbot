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
    
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_timer = 0;
    // m_encoderL.setup();
    // m_encoderR.setup();
    m_rx.setup();
    // m_imu.setup(&Wire1);
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
    m_rx.update();
    // m_encoderL.update();
    // m_encoderR.update();

    double channels[TX_NUM_CHANNELS];
    memset(channels, 0, TX_NUM_CHANNELS*sizeof(double));
    
    double steering = channels[0];
    steering = floatMap(steering, 0, 1, -1, 1)*255.0;
    
    // double throttle = channels[2]*255;

    if (!m_rx.isSafetyOff() || m_rx.hasLostConnection()) {
        m_driverL.setThrottle(0);
        m_driverR.setThrottle(0);
        // m_driverL.wakeup(false);
        // m_driverR.wakeup(false);
    }

    if (millis() - m_timer > 500) {
        m_timer = millis();
        Serial.println("Trigger");
        m_status.toggleGreen();
    }

}
