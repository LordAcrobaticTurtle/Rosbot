#pragma once

#include <localisation/localisation.h>
#include <control/control.h>
#include <drivers/RGB_LED.h>


class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void run();

        void ActivateStandbyMode();
        void ActivateCalibration();
        void ActivateControlMode();

    protected:
        void toggleLocalisation (bool isLocalisationOn);
        void toggleControl (bool isControlOn);

    private:

        bool m_isStandbyOn;
        bool m_isLocalisationOn;
        bool m_isControlOn;
        
        RGBLED m_status;
        std::shared_ptr<Localisation> m_localisation;
        std::shared_ptr<Control> m_control;
};
