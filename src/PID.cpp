#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
	Kp = Kp_;
	Ki = Ki_;
	Kd = Kd_;
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
	i_error += err;
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