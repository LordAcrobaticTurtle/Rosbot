#pragma once
#include <SBUS.h>

#define CHANNEL_THROTTLE 2
#define CHANNEL_ROLL 0
#define TX_NUM_CHANNELS 16

class RadioInterface {
    // Functions
    public:
        RadioInterface();
        ~RadioInterface();
        void setup();
        void update();
        void getChannels(uint16_t *p_channelValues);
        void getChannelEndpoints(uint16_t *p_minPoints, uint16_t * p_maxPoints);
    
    // Functions
    private:
        bool hasLostConnection();
        void readSBUS();
        void readEndPoints(uint16_t * p_endPointsMax, uint16_t * p_endPointsMin);
        void applyDeadbandToChannels(uint16_t * p_currValues, uint16_t * p_prevValues, int p_deadband);
        void scaleChannels(uint16_t * p_currValues, uint16_t * p_minValues, uint16_t * p_maxValues);

    // Member variables
    private:
        SBUS m_tx;
        bool m_failsafe;
        bool m_lostFrame;       
        int m_deadband;

        uint16_t m_channelValues[TX_NUM_CHANNELS];
        uint16_t m_prevChannelValues[TX_NUM_CHANNELS];
        uint16_t m_channelEndpointsMax[TX_NUM_CHANNELS];
        uint16_t m_channelEndpointsMin[TX_NUM_CHANNELS];


};


