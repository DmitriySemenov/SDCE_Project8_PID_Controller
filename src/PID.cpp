#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_, double dz_start, double dz_end) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
	Kp = Kp_;
	Ki = Ki_;
	Kd = Kd_;
	i_dz_start = dz_start;
	i_dz_end = dz_end;
	p_error = 0;
	i_error = 0;
	d_error = 0;
	err_prev = -999999;
}

void PID::UpdateError(double err) {
  /**
   * TODO: Update PID errors based on cte.
   */
	if (err_prev == -999999)
		err_prev = err;
	d_error = err - err_prev;
	p_error = err;
	if (err > i_dz_end)
		i_error += (err - i_dz_end);
	else if (err < i_dz_start)
		i_error += (err - i_dz_start);
	err_prev = err;

}

double PID::UpdateControl() {
	double control = -Kp * p_error - Kd * d_error - Ki * i_error;
	return control;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return 0.0;  // TODO: Add your total error calc here!
}