#pragma once
#include <stdio.h>

typedef struct PIDParams {
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