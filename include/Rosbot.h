#pragma once

#include <control/pid_controller.h>
#include <control/torque_control.h>
#include <interfaces/interfaces.h>
#include <drivers/RGB_LED.h>
#include <drivers/radio_interface.h>
#include <utility/math.h>
#include <utility/data_types.h>
#include <utility/quaternion.h>

// Returns the system state to the app
struct ControlResponse {
    size_t controlIDPlaceholder;
    PIDParams params;
    double controlResponse;
};

struct ModelControlResponse {
    float stateBuffer[4];
    float inputForce;
    float desiredVelocity;
};

struct LocalisationResponse {
    vector3D orientation;
    vector3D gyroRates;
    vector3D accelReadings;
    vector2D encoderVelocities;
    vector2D encoderPositions;
};

struct VerifiedSensorData {
    long int positionLeftRaw;
    long int positionRightRaw;
    float velocityAvgLeft;
    float velocityAvgRight;

    void toString (char *buffer) {
        sprintf(buffer, "%ld,%ld,%f,%f", 
            positionLeftRaw, positionRightRaw, velocityAvgLeft, velocityAvgRight
        );
    }
};

class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void run();

        void ActivateStandbyMode();
        void ActivateCalibration();
        void ActivateControlMode();

        LocalisationResponse getLocalisationResponse();
        ControlResponse getControlResponse();
        ModelControlResponse getModelControlResponse ();
        void resetImu ();
        vector3D getAngleOffsets ();

        PIDParams getAnglePIDParams();
        void setAnglePIDParams(PIDParams params);

        PIDParams getPositionPIDParams ();
        void setPositionPIDParams (PIDParams params);

        void setIsRadioConnected (bool isRadioConnected);

        void setMotorPosition (int motorIndex, int throttle);

        VerifiedSensorData sensorVerification (int motorIndex, float throttle, float time);


    protected:
        void runOffsetEstimation ();
        void setLocalisationMode (bool isLocalisationOn);
        void setControlMode (bool isControlOn);
        void runLocalisation ();
        void runControl ();
        void cascadedControl ();

    protected:
        // Driver related components
        RGBLED m_status;
        std::shared_ptr<ImuInterface> m_imu;
        std::shared_ptr<EncoderInterface> m_encoderL;
        std::shared_ptr<EncoderInterface> m_encoderR;
        std::shared_ptr<DcMotorInterface> m_motorL;
        std::shared_ptr<DcMotorInterface> m_motorR;
        std::shared_ptr<RadioInterface> m_rx;

        // State estimation related data.
        ImuData m_imuData;
        ImuData m_zeroOffsetData;
        
        quaternion m_qEst;
        vector2D m_vwheel;

        // Control related data
        PIDParams m_anglePidParams;
        PIDParams m_positionPidParams;
        PIDParams m_motorLPositionParams;
        PIDParams m_motorRPositionParams;

        // Mode related data
        bool isRXControlOn;
        bool isAppControlOn;

        bool m_isStandbyOn;
        bool m_isControlOn;
        bool m_isLocalisationOn;
        bool m_isRadioConnected;

        vector3D m_angleOffsets;

        // Control related data
        // =========================================
        // State matrix A,
        Matrix m_A;
        // Input matrix B,
        Matrix m_B;
        // Output matrix C
        Matrix m_C;
        // feed-forward matrix D
        Matrix m_D; 
        // State vector
        Matrix m_state; 
        // Desired state matrix
        Matrix m_desiredState;

        // Derivative of state vector
        Matrix m_dstate;

        // Gain matrix K
        Matrix m_K;

        TorqueControl m_torqueControlLeft;
        TorqueControl m_torqueControlRight;

        float m_controlSignal;
        float m_desiredVelocity;
        // =========================================
};
