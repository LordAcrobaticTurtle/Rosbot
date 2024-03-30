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

    double response;
    double target;
    double currValue;
    double kp;
    double ki;
    double kd;
    double prevError;
    double errorSum;
    double dt;
    double bounds[2];

    void toString(char *buffer) {
        sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
            target, currValue, response,kp, ki, kd, prevError, errorSum, dt, bounds[0], bounds[1]
        );
    }

    // Set the fields of the struct being operated on
    void operator=(const PIDParams& newParams) {
        
        // Guard against self assignment
        if (this == &newParams) {
            return;
        }

        target = newParams.target;
        currValue = newParams.currValue;
        kp = newParams.kp;
        ki = newParams.ki;
        kd = newParams.kd;
        prevError = newParams.prevError;
        errorSum = newParams.errorSum;
        dt = newParams.dt;
        bounds[0] = newParams.bounds[0];
        bounds[1] = newParams.bounds[1];
    }
};


class PIDController {
    public:
        static float computeResponse(PIDParams &params);
    private:
        static int count;
};  

// PIDParams params