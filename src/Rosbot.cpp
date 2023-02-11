#include <Rosbot.h>

Rosbot::Rosbot() : 
    m_stepperL(AccelStepper::DRIVER, 12,14),
    m_stepperR(AccelStepper::DRIVER, 2,3)
{ 
    
}

Rosbot::~Rosbot() {}

void Rosbot::setup() 
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

    
    if (m_rx.hasLostConnection() || !m_rx.isSafetyOff()) {
        demandThrottle = 0;
        demandSteer = 0;
    } else {
        m_stepperL.setSpeed(demandThrottle + demandSteer);
        m_stepperR.setSpeed(demandThrottle - demandSteer);

        m_stepperL.runSpeed();
        m_stepperR.runSpeed();
    }

    

    Serial.println("Throttle: " + String(demandThrottle) + ", steer: " + String(demandSteer) + ", encL: " 
                    + String(m_encL.getAngle()) + ", encR: " + String(m_encR.getAngle()));
}

