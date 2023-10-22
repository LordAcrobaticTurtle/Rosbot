#pragma once
#include <utility/data_types.h>


class CommsInterface {
public:
    virtual int isDataReady() = 0;
    virtual int readBytes(byte* buffer, size_t bufferLength) = 0;
    virtual int sendBytes(byte* buffer, size_t bufferLength) = 0;
};
