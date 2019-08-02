#pragma once
class PID
{
public:
	PID(double kp, double ki, double kd);

	void clear();
	float step(float x);

	float prev_err;
	float integral;

	float kp;
	float ki;
	float kd;

	float last_step;
};