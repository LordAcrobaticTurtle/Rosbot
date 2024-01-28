#pragma once
#include <utility/data_types.h>

struct ImuData {
    vector3D gyroRates;
    vector3D accelData;
    vector3D magneticField;
    vector3D orientation;
    vector3D position;
};

class ImuInterface {
public:
    virtual int readGyroRates(vector3D &rates) = 0;
    virtual int readOrientation(vector3D &orientation) = 0;
    virtual int readAccel(vector3D &accel) = 0;
    virtual int readTemperature(float *temp) = 0;
    virtual int readMagnetField(vector3D &field) = 0;
    virtual int update(float dt) = 0;
    virtual int readImuData(ImuData &data);
};

