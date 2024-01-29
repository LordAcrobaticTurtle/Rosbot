#pragma once
#include <memory>

class SystemTimeI {
    public:
        virtual unsigned int usElapsed () = 0;
        virtual unsigned int msElapsed () = 0;
};

class HardwareTime : public SystemTimeI {
    public:
        HardwareTime();

        virtual unsigned int usElapsed () override;
        virtual unsigned int msElapsed () override;
};

// extern std::unique_ptr<SystemTimeI> g_systemTime;

void setupGlobalTimer ();


class FrequencyTimer {
    public:
        FrequencyTimer();
        FrequencyTimer(long int timeInMicroseconds);
        ~FrequencyTimer() = default;

        void setPeriod (long int timeInMicroseconds);

        void setFrequency (double freqInHz);


        /**
         * @brief Returns true if the time enough time has elapsed since the last call.
         * 
         * @return true 
         * @return false 
         */
        bool checkEnoughTimeHasPassed ();

    protected:
        long int m_time;
        long int m_lastTime;
        long int m_target;
};
