#pragma once
#include <Arduino.h>

#define ENOSPC -2
#define EARG -1



class BLETransceiver {
    public:
        BLETransceiver(HardwareSerial * hwSerial);
        
        void configure();

        int readBytes(byte * buffer, uint32_t bufferLength);
        int sendBytes(byte * buffer, uint32_t bufferLength);

    private:
        HardwareSerial *m_hwSerial;
};