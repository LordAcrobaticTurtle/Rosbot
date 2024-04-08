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

class TorqueControl {
    public:
        TorqueControl() = default;
        ~TorqueControl() = default;

        float calculatedOutputVoltage (float torqueCommand);
        void passEncoder (std::shared_ptr<EncoderInterface> encoder);
        void setHardwareParams (TorqueControlParams params);

    protected:
        std::shared_ptr<EncoderInterface> m_encoder;
        TorqueControlParams m_params;

};