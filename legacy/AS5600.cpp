#include "AS5600.h"
#ifdef __USE_I2C_DRIVER_WIRE__

double floatMap(double value, double preMin, double preMax, double postMin, double postMax);

AS5600::AS5600():
    m_aoutPin(-1),
    m_dirPin(-1),
    m_GPOPin(-1),
    m_isConnected(false)
{

}

AS5600::~AS5600() {}

int AS5600::setup(I2CMaster &interface) {
    // Assign variables
    m_interface = std::make_shared<I2CDevice>(interface, m_ADDRESS, __ORDER_BIG_ENDIAN__);
    m_isConnected = true;
    m_currTime = 0;
    m_prevTime = 0;
    return 0;
}


double AS5600::getAngle() {
    // Use integers because it is easier to do integer math and the changes will be easier to debug

    // Check if the sensor is actually connected
    if (!m_isConnected) {
        Serial.println("getAngle: encoder is not connected");
        return 0;
    }
    
    byte regAddr = registerAddresses[ANGLEL];
    byte angleData[2] = {0,0};
    for (int i = 0; i < 2; i++) {
      m_interface->read(regAddr+i, &angleData[i], true);
    }


    m_angleRaw = angleData[0] << 8 | angleData[1];
    
    m_angle = floatMap(m_angleRaw, 0, 4096, 0, 360);

    return m_angle;
}

unsigned char AS5600::getStatus() {

    if (!m_isConnected) 
        return 0;

    byte regAddr = registerAddresses[STATUS];
    byte status = 0;
    int error = m_interface->read(regAddr, &status, true);

    if (error == -1) 
        Serial.println("Status: Error in request data");
    
    m_status = status;
    return m_status;
}

unsigned char AS5600::getAGC() {
    
    if (!m_isConnected)
        return 0;
        
    byte regAddr = registerAddresses[AGC];
    byte response = 0;
    
    int error = m_interface->read(regAddr, &response, true);
    
    if (error == -1) 
        Serial.println("AGC: Error in request data");

    m_AGC = response;
    return m_AGC;
}

double AS5600::getVelocity() {
  // Compare current position and current timestamp with prev position and timestamp.
  // Linearly interpolate between the two of them. 
  static long int funcTimer = 0;
  m_currTime = millis();

  if (m_currTime - funcTimer < 1) {
    funcTimer = m_currTime;
    return m_currVelocity;
  }
  
  // Calculate change in position
  double diffAngleDegrees = (double) m_angle - m_prevAngle;
  double diffTime = (double) m_currTime - m_prevTime;

  diffTime *= 1e-3;

  m_prevAngle = m_angle;
  m_prevTime = m_currTime;
  m_currVelocity = diffAngleDegrees / diffTime;

  return m_currVelocity;
}

double floatMap(
    double value, double preMin, double preMax, 
    double postMin, double postMax) 
{
    return (postMax - postMin)/(preMax - preMin) * (value - preMin) + postMin;
}


#endif