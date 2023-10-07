// Defines Motor class
#include <Arduino.h>

#ifndef MOTOR_h
#define MOTOR_h

class Motor
{
    private:
        bool side; // 0 for left, 1 for right
        int PWM_pin;
        int forward_pin;
        int backward_pin;
    
    public:
        Motor(int PWM, int forward, int backward);
        void set(char direction, unsigned int speed = 0);
};

#endif