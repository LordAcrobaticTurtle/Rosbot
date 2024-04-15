#pragma once
#include "interfaces/encoder_interface.h"
#include <memory>

/**
 * @brief Separate interface used such that it can be applied across any motor model
 */

struct TorqueControlParams {
    float motorResistance;
    float motorInductance;
    float Kt;
};

/**
 * @brief This class is used to perform torque control on a motor. It takes hardware params like 
 * resistance, inductance an torque constant then computes a voltage value to apply to the rotor. 
 */
class TorqueControl {
    public:
        TorqueControl() = default;
        ~TorqueControl() = default;

        float calculatedOutputVoltage (float torqueCommand);
        void passEncoder (std::shared_ptr<EncoderInterface> encoder);
        void setHardwareParams (TorqueControlParams params);
        void resetTorqueController ();

    protected:
        std::shared_ptr<EncoderInterface> m_encoder;
        TorqueControlParams m_params;

};