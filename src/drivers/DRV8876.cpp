#include <drivers/DRV8876.h>
#include <utility/math.h>
#include <control/pid_controller.h>

DRV8876::DRV8876(
    int pinEN, int pinPH, int pinCurrSense, 
    int pinNFault, int pinSleep
) {
    m_pinPWM = pinEN;
    m_pinDirection = pinPH;
    m_pinCurrSense = pinCurrSense;
    m_pinNFault = pinNFault;
    m_pinSleep = pinSleep;
    m_batteryVoltage = 0;

    pinMode(m_pinPWM, OUTPUT);
    pinMode(m_pinDirection, OUTPUT);
    pinMode(m_pinCurrSense, INPUT);
    pinMode(m_pinNFault, OUTPUT);
    pinMode(m_pinSleep, OUTPUT);
    wakeup(true);
    
    setThrottle(0);
}

// Model the motor
// Find a gains matrix K
// Set target current 
void DRV8876::wakeup(bool setAwake) {
    digitalWrite(m_pinSleep, setAwake);
}

void DRV8876::setVoltage(float voltage) {
    // Scale voltage to something between 
    if (m_batteryVoltage == 0) {
        return;
    }

    float scale = voltage / m_batteryVoltage;
    
    return;
}

void DRV8876::setBatteryVoltage (float voltage) {
    m_batteryVoltage = voltage;
}

// Expects values between -255 and 255
void DRV8876::setThrottle(int throttle) {

    throttle = constrain(throttle, MIN_THROTTLE, MAX_THROTTLE);
    
    if (throttle < 0) {
        setDirection(false);
    } else {
        setDirection(true);
    }

    analogWrite(m_pinPWM, abs(throttle));
}

void DRV8876::setDirection(bool isClockwise) {
    digitalWrite(m_pinDirection, isClockwise);
    m_isClockwise = isClockwise;
}

double DRV8876::readCurrent() {

    int currentAsInteger = analogRead(m_pinCurrSense);

    // Convert to voltage
    double currentAsVoltage = floatMap(currentAsInteger, ADC_MIN, ADC_MAX, VOLTAGE_MIN, VOLTAGE_MAX);
    double I_propI = currentAsVoltage / R_IPROPI; // microamps
    m_current = I_propI;
    return I_propI; // Amps
}

void DRV8876::setCurrent (double current) {
    m_current = current;
}

int DRV8876::readCurrentAnalog() {
    return analogRead(m_pinCurrSense);
}


void DRV8876::setPosition (PIDParams &params) {
    runPositionControl(params);
}

void DRV8876::run () {
    // Run position control update
}

void DRV8876::runPositionControl (PIDParams &params) {
    // Assume the PID params struct has been updated appropriately 
    // Use a PID controller. Compare the encoder position and the desired position. 
    
    // Bounds should be [-1,1]
    float response = PIDController::computeResponse(params);
    char buffer[256];
    params.toString(buffer);
    Serial.println(buffer);
    // Generate a throttle value
    int scaledResponse = response * 255.0;

    // Drive currValue to set point 
    setThrottle(-scaledResponse);
}
