#pragma once
#include <utility/data_types.h>

class ImuInterface {
public:
    // ImuInterface() {};
    // virtual ~ImuInterface() {};
    virtual int readGyroRates(vector3D &rates) = 0;
    virtual int readAccel(vector3D &accel) = 0;
    virtual int readTemperature(float *temp) = 0;
    virtual int readMagnetField(vector3D &field) = 0;
};
