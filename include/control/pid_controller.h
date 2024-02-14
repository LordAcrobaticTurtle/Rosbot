#pragma once
#include <stdio.h>

// target, currValue, bounds, dt + PID gains must be set.
typedef struct PIDParams {

    PIDParams() {
        target = 0;
        currValue = 0;
        kp = 0;
        ki = 0;
        kd = 0;
        prevError = 0;
        errorSum = 0;
    }

    float target;
    float currValue;
    float kp;
    float ki;
    float kd;
    float prevError;
    float errorSum;
    float dt;
    float bounds[2];

    void toString(char *buffer) {
        sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
            target, currValue, kp, ki, kd, prevError, errorSum, dt, bounds[0], bounds[1]
        );
    }
} PIDParams;


class PIDController {
    public:
        static float computeResponse(PIDParams &params);
    private:
        static int count;
};  

// PIDParams params