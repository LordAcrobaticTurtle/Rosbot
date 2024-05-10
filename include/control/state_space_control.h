#pragma once

#include <utility/math.h>

/**
 * @brief A general class used to calculate the control signal of a full state feedback controller.
 * You must know what this is being used for in advance. The state vector rows and columns are passed on 
 * contruction.
 */
class StateSpaceControl {
    
    public:
        StateSpaceControl(int stateRows, int stateCols);
        ~StateSpaceControl() = default;

        void setGainMatrix (Matrix gains);

        virtual float calculateControlSignal (Matrix desiredState, Matrix state);

    protected:
        Matrix m_state;
        Matrix m_desiredState;
        Matrix m_K;

        float m_integrator;
        float m_integratorGain;
};
