#include "control/pid_controller.h"
#include <Arduino.h>

float PIDController::computeResponse(PIDParams &params) {
    float error = params.currValue - params.target;
    float derror = (error - params.prevError)/params.dt;
    params.errorSum += error*params.dt;
    float response = params.kp * error + params.kd * derror + params.ki * params.errorSum;
    response = constrain(response, params.bounds[0], params.bounds[1]);
    params.errorSum = constrain(params.errorSum, params.bounds[0], params.bounds[1]);
    params.prevError = error;
    // Serial.println("ErrorSum = " + String(params.errorSum) + ", i term" + String(params.ki*params.errorSum));
    return response;
}
