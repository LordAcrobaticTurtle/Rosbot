#include "control/motor_state_space_control.h"


MotorStateSpaceControl::MotorStateSpaceControl () : 
    m_K(1,2),
    m_state(2,1)
{

}

void MotorStateSpaceControl::setHardwareParams (MotorSSParams params) {
    m_hardwareParams = params;
}

void MotorStateSpaceControl::setStateVector (stateVector state) {
    m_state.data[0][0] = state.dtheta;
    m_state.data[1][0] = state.i;
}

float MotorStateSpaceControl::calculateOutputVoltage () {
    // u = - K * x
    Matrix uTemp = Matrix::multiply(m_K, m_state);
    float U = uTemp.data[0][0];
}