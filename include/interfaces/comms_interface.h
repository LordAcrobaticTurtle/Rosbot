#pragma once
#include <utility/data_types.h>

// typedef long unsigned int size_t;

class CommsInterface {
public:
    virtual int isDataReady() = 0;
    virtual int readBytes(byte* buffer, size_t bufferLength) = 0;
    virtual int sendBytes(byte* buffer, size_t bufferLength) = 0;
};
