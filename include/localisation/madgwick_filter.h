//
//  madgwickFilter.h
//  madgwickFilter
//
//  Created by Blake Johnson on 4/28/20.
//
#pragma once
#include "utility/quaternion.h"

// Include a hardware specific header file to redefine these predetermined values
#ifndef DELTA_T
    #define DELTA_T 0.01f // 100Hz sampling frequency
#endif

#ifndef PI  
    #define PI 3.14159265358979f
#endif

#ifndef GYRO_MEAN_ERROR
    #define GYRO_MEAN_ERROR PI * (5.0f / 180.0f) // 5 deg/s gyroscope measurement error (in rad/s)  *from paper*
#endif

#ifndef BETA
    #define BETA sqrt(3.0f/4.0f) * GYRO_MEAN_ERROR    //*from paper*
#endif


// IMU consists of a Gyroscope plus Accelerometer sensor fusion
void imu_filter(float ax, float ay, float az, float gx, float gy, float gz, quaternion &q_est);


void eulerAngles(struct quaternion q, float* roll, float* pitch, float* yaw);
