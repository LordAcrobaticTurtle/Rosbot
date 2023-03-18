#pragma once
#include <Arduino.h>

class BLETransceiver {
    public:
        BLETransceiver(HardwareSerial * hwSerial);
        
        void configure();

        void readBytes(byte * buffer, unsigned int bufferLength);
        void sendBytes(byte * buffer, unsigned int bufferLength);

    private:
        
        HardwareSerial *m_hwSerial;
};