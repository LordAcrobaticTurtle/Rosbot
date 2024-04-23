#include "control/state_space_control.h"

StateSpaceControl::StateSpaceControl (int stateRows, int stateCols) : 
    m_state(stateRows, stateCols),
    m_desiredState(stateRows, stateCols),
    m_K(stateCols, stateRows)
{
    
}


void StateSpaceControl::setGainMatrix (Matrix gains) {
    // Should make an assertion that they are the same size;
    gains.copyTo(m_K);
}


float StateSpaceControl::calculateControlSignal (Matrix desiredState, Matrix state) {
    
    Matrix stateDiff = Matrix::subtract(desiredState, state);
    
    // Gain matrix K * state is always going to be a 1x1 result. 
    Matrix u = Matrix::multiply(m_K, stateDiff);

    return (-u.data[0][0]);
}