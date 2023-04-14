#include <Rosbot.h>

Rosbot::Rosbot() : 
m_rx(&Serial1)
{ 
    
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
{
    m_encL.setup(&Wire);
    m_encR.setup(&Wire1);
    m_rx.setup();
}

void Rosbot::tankDrive() 
{
    // Update command values
    m_rx.update();
    m_imu.update();
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

