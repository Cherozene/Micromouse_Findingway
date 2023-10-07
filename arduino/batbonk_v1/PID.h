// Defines PID class
#ifndef PID_h
#define PID_h

class PID
{
    private:
        float Kp, Ki, Kd, Kaw;
        int setpoint;
        float err;
        float integral;
        float deriv;
        float previous_err;
        int command;
        int command_prev;
        int command_sat;
        int command_sat_prev;
    
    public:
        PID(float Kp_coeff, float Ki_coeff, float Kd_coeff, float Kaw_coeff, int command_saturation_value);
        void set_setpoint(int pid_setpoint);
        void reset();
        int compute_correction(int measure);
};

#endif