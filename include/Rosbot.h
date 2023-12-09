#pragma once

#include <localisation/localisation.h>
#include <control/control.h>
#include <drivers/RGB_LED.h>


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
        void resetImu();

    protected:
        void toggleLocalisation (bool isLocalisationOn);
        void toggleControl (bool isControlOn);

    private:
        RGBLED m_status;
        std::shared_ptr<Localisation> m_localisation;
        std::shared_ptr<Control> m_control;

        bool m_isStandbyOn;
        bool m_isLocalisationOn;
        bool m_isControlOn;
        
};
