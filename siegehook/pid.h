#pragma once

#include "math.h"

class PIDController
{
public:
	float Kp;
	float Ki;
	float Kd;
	float integral_error;
	float last_error;

	float calculate_correction(float target, float current, float delta);
};

