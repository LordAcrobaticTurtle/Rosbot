#include "control/pid_controller.h"

float PIDController::computeResponse(PIDParams &params) {
    float error = params.currValue - params.target;
    float derror = (error - params.prevError)/params.dt;
    params.errorSum += error*params.dt;
    float response = params.kp * error + params.kd * derror + params.ki * params.errorSum;
    params.prevError = error;
    return response;
}
