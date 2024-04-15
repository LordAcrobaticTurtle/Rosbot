#include "control/torque_control.h"


float TorqueControl::calculatedOutputVoltage (float torqueCommand) {
    float voltage = 100.0*torqueCommand * m_params.Kt / m_params.motorResistance + 
                    m_params.motorResistance * m_encoder->readRadsPerSecond();
    return voltage;
}

void TorqueControl::passEncoder (std::shared_ptr<EncoderInterface> encoder) {
    m_encoder = encoder;
}

void TorqueControl::setHardwareParams (TorqueControlParams params) {
    m_params = params;
}

void TorqueControl::resetTorqueController () {
    
}