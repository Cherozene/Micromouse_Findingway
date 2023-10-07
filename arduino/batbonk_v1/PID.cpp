// PID class code
#include "PID.h"

PID::PID(float Kp_coeff, float Ki_coeff, float Kd_coeff, float Kaw_coeff, int command_saturation_value)
{
    /* 
     * Inits the PID with Kp, Ki and Kd, and the max value of the command
     */
    Kp = Kp_coeff;
    Ki = Ki_coeff;
    Kd = Kd_coeff;
    Kaw = Kaw_coeff;
    command = 0;
    command_prev = 0;
    command_sat = command_saturation_value;
    command_sat_prev = 0;
    err = 0;
    previous_err = 0;
    integral = 0;
    deriv = 0;
}

void PID::set_setpoint(int pid_setpoint)
{
    /* 
     * Sets the PID setpoint
     */
    setpoint = pid_setpoint;
}

void PID::reset()
{
    /* 
     * Resets the PID errors
     */
    previous_err = 0;
    integral = 0;
}

int PID::compute_correction(int measure)
{
    /* 
     * Calculates the correction, given the measure
     */
    err = setpoint - measure;
    integral += Ki * err + Kaw * (command_sat_prev - command_prev);
    deriv = err - previous_err;

    command = (int) Kp * err + Ki * integral + Kd * deriv;
    command_prev = command;
    if (command > command_sat)
    {
        command = command_sat;
    }
    else if (command < -command_sat)
    {
        command = -command_sat;
    }
    command_sat_prev = command_sat;
    previous_err = err;
    return command;
}