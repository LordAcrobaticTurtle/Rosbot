#pragma once
#include <Arduino.h>
#include <interfaces/comms_interface.h>
#define ENOSPC -2
#define EARG -1


/**
 * @brief Basically a wrapper around a serial port. Nothing too special 
 * 
 */
class BluetoothTransceiver : public CommsInterface {
    public:
        BluetoothTransceiver();

        void init(HardwareSerial* hwSerial, long int baudrate);
        void configure();

        virtual int isDataReady();
        virtual int readBytes(byte* buffer, size_t bufferLength);
        virtual int sendBytes(byte* buffer, size_t bufferLength);

    private:
        HardwareSerial* m_hwSerial;
};