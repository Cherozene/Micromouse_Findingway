// Motor class code
#include "motor.h"

Motor::Motor(int PWM, int forward, int backward)
{
    /* 
     * Inits a motor with PWM pin, forward pin and backward pin (to drive the H-bridge)
     */
    PWM_pin = PWM;
    forward_pin = forward;
    backward_pin = backward;
    pinMode(PWM_pin, OUTPUT);
    analogWrite(PWM_pin, 0);
    digitalWrite(forward_pin, LOW);
    digitalWrite(backward_pin, LOW);
}

void Motor::set(char direction, unsigned int speed = 0)
{
    /* 
     * Sets a motor direction (Forward, Backward or Stop) and its speed
     */

    // moving Forward
    if (direction == 'F')
    {
        analogWrite(PWM_pin, speed);
        digitalWrite(forward_pin, HIGH);
        digitalWrite(backward_pin, LOW);
    }
    // moving Backward
    else if (direction == 'B')
    {
        analogWrite(PWM_pin, speed);
        digitalWrite(forward_pin, LOW);
        digitalWrite(backward_pin, HIGH);
    }
    // Stop motors
    else if (direction == 'S')
    {
        analogWrite(PWM_pin, 0);
        digitalWrite(forward_pin, LOW);
        digitalWrite(backward_pin, LOW);
    }
    // wrong char
    else
    {
        //
    }
}







