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

        void toggleStandbyMode (bool isStandByOn);
        void toggleCalibration(bool isCalibrationOn);

    protected:
        void toggleLocalisation (bool isLocalisationOn);
        void toggleControl (bool isControlOn);

    private:

        bool m_isStandbyOn;

        RGBLED m_status;
        std::shared_ptr<Localisation> m_localisation;
        std::shared_ptr<Control> m_control;
};
