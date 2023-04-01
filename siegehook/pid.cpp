#include "pid.h"

// Run the PID controller.
// Arguments:
//  target: The target value to reach
//  current: The current value
//  delta: The time since the last call to calculate_correction
float PIDController::calculate_correction(float target, float current, float delta)
{
    float error = target - current;
    integral_error += error * delta;
    float derivative_error = (error - last_error) / delta;
    last_error = error;
    
    float output = (Kp * error) + (Ki * integral_error) + (Kd * derivative_error);
    
    return output;
}
