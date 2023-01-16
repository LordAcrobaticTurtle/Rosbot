#include <AS5600.h>
#include "utility.h"


AS5600::AS5600():
    m_aoutPin(-1),
    m_dirPin(-1),
    m_GPOPin(-1)
{

}

AS5600::~AS5600() {}

int AS5600::setup(I2CDriverWire *p_wirePtr) {
    // Assign variables
    // Test I2C comms
    // Check if the sensor exists
    // Use technique from i2c scanner
    m_wireObj = p_wirePtr;

    if (m_wireObj == NULL) {
        Serial.println("Setup: m_wireObj is null");
        while (true) {}
    }

    m_wireObj->begin();
    m_wireObj->beginTransmission(m_ADDRESS);
    
    // A successfully detected device will return a 0 on end transmission
    char error = m_wireObj->endTransmission();
    // char error = 0;

    if (error != 0) {
        Serial.print("Sensor not attached, error: " + String(error));
        m_isConnected = false;
    }

    m_isConnected = true;
    
    return 0  ;
}

int AS5600::requestData(unsigned int p_numBytesRequested, unsigned char * p_buffer, unsigned int p_bufferSize) {
    return i2c_read(m_wireObj, m_ADDRESS, p_numBytesRequested, p_buffer, p_bufferSize);
}

int AS5600::writeData(unsigned char * p_whichRegisters, unsigned int p_bufferSize) {
    return i2c_write(m_wireObj, m_ADDRESS, p_whichRegisters, p_bufferSize, false);
}

unsigned int AS5600::getAngle() {
    
    // Check if the sensor is actually connected
    if (!m_isConnected) {
        Serial.println("getAngle: encoder is not connected");
        return 0;
    }
    
    unsigned char reg = registerAddresses[ANGLEL];
    writeData(&reg, 1);
    unsigned char angleData[2] = {0,0};
    int error = requestData(2, angleData, 2);
    if (error == -1) 
        Serial.println("Error in request data");

    m_angleRaw = angleData[0] << 8 | angleData[1];
    
    return m_angleRaw;
}

unsigned char AS5600::getStatus() {

    if (!m_isConnected) 
        return 0;

    unsigned char reg = registerAddresses[STATUS];
    unsigned char status = 0;
    writeData(&reg, 1);
    int error = requestData(1,&status,1);
    
    if (error == -1) 
        Serial.println("Status: Error in request data");
    
    m_status = status;
    return m_status;
}

unsigned char AS5600::getAGC() {
    
    if (!m_isConnected)
        return 0;
        
    unsigned char reg = registerAddresses[AGC];
    unsigned char response = 0;
    writeData(&reg,1);
    int error = requestData(1,&response, 1);
 
    if (error == -1) 
        Serial.println("AGC: Error in request data");

    m_AGC = response;
    return m_AGC;
}