#pragma once
#include <interfaces/interfaces.h>
#include <memory>
#include <utility/math.h>

struct MotorSSParams {
    float R,L,K,J,b;
};

struct stateVector {
    float dtheta;
    float i;
};

class MotorStateSpaceControl {
    public:
        MotorStateSpaceControl ();
        ~MotorStateSpaceControl () = default;

        void setHardwareParams (MotorSSParams params);
        void setStateVector (stateVector state);

        float calculateOutputVoltage ();

    protected:
        Matrix m_K;
        Matrix m_state;
        MotorSSParams m_hardwareParams;

};