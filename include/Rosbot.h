#pragma once


#include <localisation/localisation.h>
#include <comms/comms.h>
#include <control/control.h>

#include <drivers/RGB_LED.h>
#include <drivers/RadioInterface.h>


class Rosbot {
    public:
        Rosbot();
        ~Rosbot();

        void setup();
        void run();

    private:

        double m_channels[TX_NUM_CHANNELS];
        RadioInterface m_rx;
        RGBLED m_status;
        
        std::shared_ptr<Localisation> m_localisation;
        std::shared_ptr<Control> m_control;
        std::shared_ptr<Comms> m_comms;

};
