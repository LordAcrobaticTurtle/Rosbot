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


#define HZ_1000_MICROSECONDS 1000
#define HZ_500_MICROSECONDS  2000
#define HZ_200_MICROSECONDS  5000
#define HZ_100_MICROSECONDS  10000
#define HZ_50_MICROSECONDS   20000
#define HZ_1_MICROSECONDS    1000000

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
