#include <Rosbot.h>

Rosbot::Rosbot() : 
    m_stepperL(MOTORINTERFACETYPE, m_stepPinL, m_dirPinL),
    m_stepperR(MOTORINTERFACETYPE, m_stepPinR, m_dirPinR)
{ 
    m_encL.setup(&Wire);
    m_encR.setup(&Wire1);
    m_rx.setup();

    // Configure stepper motors
    m_stepperMaxSpeed = 400;
    m_stepperMaxAccel = 80;
    m_stepperL.setMaxSpeed(m_stepperMaxSpeed);
    m_stepperR.setMaxSpeed(m_stepperMaxSpeed);
    m_stepperL.setAcceleration(m_stepperMaxAccel);
    m_stepperR.setAcceleration(m_stepperMaxAccel);
}

Rosbot::~Rosbot() {}



void Rosbot::targetStepperPosL(int target) 
{
    m_stepperL.moveTo(target);
}


void Rosbot::targetStepperPosR(int target) 
{
    m_stepperR.moveTo(target);
}

void Rosbot::stepperRun() 
{
    m_stepperL.run();
    // m_stepperR.run();
}

void Rosbot::tankDrive() 
{
    // Update command values
    m_rx.update();
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

    float demandThrottle = map(throttle, throttleMin, throttleMax, 
                                0, m_stepperMaxSpeed);
    
    float demandSteer = map(roll, rollMin, rollMax, 
                            -m_stepperMaxSpeed/2, m_stepperMaxSpeed/2);

    m_stepperL.setSpeed(demandThrottle + demandSteer);
    m_stepperR.setSpeed(demandThrottle - demandSteer);

    Serial.println("Throttle: " + String(demandThrottle) + ", steer: " + String(demandSteer) + ", encL: " 
                    + String(m_encL.getAngle()) + ", encR: " + String(m_encR.getAngle()));

    m_stepperL.runSpeed();
    m_stepperR.runSpeed();
}

