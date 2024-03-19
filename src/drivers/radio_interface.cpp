#include <drivers/radio_interface.h>
#include <Arduino.h>

RadioInterface::RadioInterface(HardwareSerial *p_serialPort) : 
m_tx(*p_serialPort)
{
    m_deadband = 50;
    m_isInitialised = true;
    m_isDeadSwitchOff = false;
}

RadioInterface::~RadioInterface() {}

void RadioInterface::setup() 
{
    m_tx.begin();
    readEndPoints(m_channelEndpointsMax, m_channelEndpointsMin);
}

void RadioInterface::readEndPoints(uint16_t *p_endPointsMax, uint16_t *p_endPointsMin)
{
    for (int i = 0; i < TX_NUM_CHANNELS; i++) {
        m_tx.getEndPoints(i,&p_endPointsMin[i],&p_endPointsMax[i]);
    }    
    // while (1) {
    //     for (int i = 0; i < TX_NUM_CHANNELS; i++) {
    //         Serial.print("For ch[" + String(i) + "], min:" + String(p_endPointsMin[i]) + ", max:" + String(p_endPointsMax[i]));
    //     }
    //     Serial.println();
    // }
}

void RadioInterface::getChannels(uint16_t *p_channelValues) 
{
    memcpy(p_channelValues, m_channelValues, sizeof(uint16_t)*TX_NUM_CHANNELS);
}

void RadioInterface::getChannelEndpoints(uint16_t * p_minEndpoints, uint16_t * p_maxEndpoints) 
{
    memcpy(p_minEndpoints, m_channelEndpointsMin, sizeof(uint16_t)*TX_NUM_CHANNELS);
    memcpy(p_maxEndpoints, m_channelEndpointsMax, sizeof(uint16_t)*TX_NUM_CHANNELS);
}

int RadioInterface::getChannelPercentage(double *p_channelValues, double scaleFactor) {

    if (!m_isInitialised) return -1;

    for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) {
        p_channelValues[ch] = (double) m_channelValues[ch]/1000.0 * scaleFactor;
    }

    return 0;
}

// Called publicly.
void RadioInterface::run() 
{
    // Read new values into currChannelValues
    int result = readSBUS();
    if (result == -1) return;

    if (!m_isInitialised) return;

    if (hasLostConnection()) {
        // Set all data to zero
        for(int ch = 0; ch < TX_NUM_CHANNELS; ch++) 
            m_channelValues[ch] = 0;
        return;
    }

    // Apply deadband
    // If curr value is outside prev value + deadband. Leave it alone
    scaleChannels(m_channelValues, m_channelEndpointsMin, m_channelEndpointsMax);
    int deadband = 5;
    applyDeadbandToChannels(m_channelValues, m_prevChannelValues, deadband);

    if (m_channelValues[7] > 500) {
        m_isDeadSwitchOff = true;
    } else {
        m_isDeadSwitchOff = false;
    }

}

bool RadioInterface::hasLostConnection() 
{
    return m_failsafe;
}

bool RadioInterface::isSafetyOff()
{
    // return m_isDeadSwitchOff;
    return m_channelValues[CHANNEL_SAFETY] > 500;
}

int RadioInterface::readSBUS() 
{
    // TODO - Memcpy
    for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) 
        m_prevChannelValues[ch] = m_channelValues[ch];

    bool result = m_tx.read(m_channelValues, &m_failsafe, &m_lostFrame);
    
    if (!result) {
        return -1;
    }

    return 0;
}



void RadioInterface::applyDeadbandToChannels(uint16_t * p_currChannelValues, uint16_t  * p_prevChannelValues, int p_deadband) 
{
    for(int ch = 0 ; ch < TX_NUM_CHANNELS; ch++) {
        if ((p_currChannelValues[ch] < p_prevChannelValues[ch] + p_deadband) &&
            (p_currChannelValues[ch] > p_prevChannelValues[ch] - p_deadband)) {
                // Do not update the current value.
                // Set it to be the previous value
                p_currChannelValues[ch] = p_prevChannelValues[ch];
        }
    }
}

void RadioInterface::scaleChannels(uint16_t *p_currValues, uint16_t *p_minValues, uint16_t *p_maxValues) 
{
    
    for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) {
        p_currValues[ch] = map(p_currValues[ch], RAW_CHANNEL_MIN, RAW_CHANNEL_MAX, POST_CHANNEL_MIN, POST_CHANNEL_MAX);
    }
    
    
}


void RadioInterface::printChannels() {
    for (int ch = 0; ch < 8; ch++) {
        Serial.print("ch[" + String(ch) + "] = " + String(m_channelValues[ch]) + ", ");
    }
}

 