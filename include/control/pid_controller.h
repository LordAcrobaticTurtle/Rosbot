#pragma once


typedef struct PIDParams {
    float target;
    float currValue;
    float kp;
    float ki;
    float kd;
    float prevError;
    float errorSum;
    float dt;
} PIDParams;


class PIDController {
    public:
        static float computeResponse(PIDParams &params);
    private:
        static int count;
};  

// PIDParams params