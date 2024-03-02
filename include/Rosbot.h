#pragma once

#include <localisation/localisation.h>
#include <control/control.h>
#include <drivers/RGB_LED.h>
#include <utility/quaternion.h>
#include <drivers/radio_interface.h>

struct ControlResponse {
    size_t controlIDPlaceholder;
    PIDParams params;
    double controlResponse;
};

struct LocalisationResponse {
    vector3D orientation;
    vector3D gyroRates;
    vector3D accelReadings;
    vector2D encoderVelocities;
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
        void resetImu ();
        vector3D getAngleOffsets ();

        PIDParams getAnglePIDParams();
        void setAnglePIDParams(PIDParams params);

        PIDParams getPositionPIDParams ();
        void setPositionPIDParams (PIDParams params);

        void setIsRadioConnected (bool isRadioConnected);

        void setMotorPosition (int motorIndex, int throttle);


    protected:
        void runOffsetEstimation ();
        void setLocalisationMode (bool isLocalisationOn);
        void setControlMode (bool isControlOn);
        void runLocalisation ();
        void runControl ();

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
        bool m_isStandbyOn;
        bool m_isControlOn;
        bool m_isLocalisationOn;
        bool m_isRadioConnected;

        vector3D m_angleOffsets;


        
};
