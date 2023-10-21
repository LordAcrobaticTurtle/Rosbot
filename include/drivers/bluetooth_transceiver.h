#pragma once
#include <Arduino.h>

#define ENOSPC -2
#define EARG -1


/**
 * @brief Basically a wrapper around a serial port. Nothing too special 
 * 
 */
class BluetoothTransceiver {
    public:
        BluetoothTransceiver();

        void init(HardwareSerial* hwSerial, long int baudrate);
        void configure();

        int readBytes(byte* buffer, uint32_t bufferLength);
        int sendBytes(byte* buffer, uint32_t bufferLength);

    private:
        HardwareSerial* m_hwSerial;
};