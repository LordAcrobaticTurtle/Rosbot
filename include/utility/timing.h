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

