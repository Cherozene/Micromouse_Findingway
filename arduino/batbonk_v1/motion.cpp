// Motion functions
#include "motor.h"
#include "IR.h"
#include "motion.h"

void stop_motors(Motor MotorRight, Motor MotorLeft)
{
    /* 
     * Stops motors
    */
    MotorRight.set('S');
    MotorLeft.set('S');
}

void move_forward(Motor MotorRight, Motor MotorLeft, unsigned int speed_right, unsigned int speed_left)
{
    /* 
     * Sets motors to move forward at designated speed
    */
    MotorRight.set('F', speed_right); 
    MotorLeft.set('F', speed_left);
}

void move_backward(Motor MotorRight, Motor MotorLeft, unsigned int speed_right, unsigned int speed_left)
{
    /* 
     * Sets motors to move forward at designated speed
    */
    MotorRight.set('B', speed_right); 
    MotorLeft.set('B', speed_left);
}

void turn_right_90(Motor MotorRight, Motor MotorLeft, 
                  unsigned int speed_right, unsigned int speed_left,
                  int* encoder_right_counter, int* encoder_left_counter,
                  int angle = 90)
{
    /* 
     * Sets motors to perform a 90 degrees right turn at designated speed
    */

    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    MotorRight.set('B', speed_right); 
    MotorLeft.set('F', speed_left);     
    //while ((*encoder_right_counter > -enc_counts) || (*encoder_left_counter < enc_counts))
    while ( (DEG_PER_COUNT * (*encoder_left_counter - *encoder_right_counter)) < 90)
    {
        delay(1); // TODO: ce délai me déplaît
    }
    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    stop_motors(MotorRight, MotorLeft);
}

void turn_left_90(Motor MotorRight, Motor MotorLeft, 
                  unsigned int speed_right, unsigned int speed_left,
                  int* encoder_right_counter, int* encoder_left_counter,
                  int angle = 90)
{
    /* 
     * Sets motors to perform a 90 degrees left turn at designated speed
    */

    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    MotorRight.set('F', speed_right); 
    MotorLeft.set('B', speed_left);           
    while ( (DEG_PER_COUNT * (*encoder_right_counter - *encoder_left_counter)) < 90)
    {
        delay(1); // TODO: ce délai me déplaît
    }
    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    stop_motors(MotorRight, MotorLeft);
    
}

void forward_one_step(Motor MotorRight, Motor MotorLeft, 
                     unsigned int speed_right, unsigned int speed_left,
                     int* encoder_right_counter, int* encoder_left_counter)
{
    /* 
     * Sets motors to perform one step forward at designated speed
    */

    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    MotorRight.set('F', speed_right); 
    MotorLeft.set('F', speed_left);           
    while ((*encoder_right_counter < 200) || (*encoder_left_counter < 200))
    {
        delay(1); // TODO: ce délai me déplaît
    }
    *encoder_right_counter = 0;
    *encoder_left_counter = 0;
    stop_motors(MotorRight, MotorLeft);
    
}

void uturn(IRSensors IR, Motor MotorRight, Motor MotorLeft, 
           unsigned int speed_right, unsigned int speed_left,
           int* encoder_right_counter, int* encoder_left_counter)
{
    /* 
     * Sets motors to perform a u-turn
    */
    char closest_side; // dans quel sens tourner pour faire demi tour
    closest_side = IR.closest_wall();
    if (closest_side == 'R')
    {
        *encoder_left_counter = 0;
        move_backward(MotorRight, MotorLeft, 0, 120);         
        while ((*encoder_left_counter > -250))
        {
            delay(1); // TODO: ce délai me déplaît
        }
        *encoder_right_counter = 0;
        move_forward(MotorRight, MotorLeft, 120, 0);         
        while ((*encoder_right_counter < 210))
        {
            delay(1); // TODO: ce délai me déplaît
        }
        *encoder_right_counter = 0;
        *encoder_left_counter = 0;   
        move_backward(MotorRight, MotorLeft, 120, 120);
        delay(600);
        
    }
    else // closest_side == 'L'
    {
        *encoder_right_counter = 0;
        move_backward(MotorRight, MotorLeft, 120, 0);         
        while ((*encoder_right_counter > -210))
        {
            delay(1); // TODO: ce délai me déplaît
        }
        *encoder_left_counter = 0;
        move_forward(MotorRight, MotorLeft, 0, 120);         
        while ((*encoder_left_counter < 250))
        {
            delay(1); // TODO: ce délai me déplaît
        }
        *encoder_right_counter = 0;
        *encoder_left_counter = 0;       
        move_backward(MotorRight, MotorLeft, 120, 120);
        delay(600);
    }
}

char define_next_move(char lookingat, char next_direction)
{
    /* 
     * According to current direction, and next direction, 
     * returns the move to perform ('L' for turn left, 'R', 'U' for uturn, 'F' for forward (keep same dir))
    */
    char next_move = 'F'; 
    // si on regarde au nord
    if (lookingat == 'N')
    {
        // et qu'on doit aller à l'est
        if (next_direction == 'E')
        {
            // on tourne à droite
            next_move = 'R';
        }
        // et qu'on doit aller au sud
        else if (next_direction == 'S')
        {
            // on fait demi tour
            next_move = 'U';
        }
        // et qu'on doit aller à l'ouest
        else if (next_direction == 'W')
        {
            // on tourne à gauche
            next_move = 'L';
        }
    }
    // si on regarde à l'est
    else if (lookingat == 'E')
    {
        // et qu'on doit aller au sud
        if (next_direction == 'S')
        {
            // on tourne à droite
            next_move = 'R';
        }
        // et qu'on doit aller à l'ouest
        else if (next_direction == 'W')
        {
            // on fait demi tour
            next_move = 'U';
        }
        // et qu'on doit aller au nord
        else if (next_direction == 'N')
        {
            // on tourne à gauche
            next_move = 'L';
        }
    }
    // si on regarde au sud
    else if (lookingat == 'S')
    {
        // et qu'on doit aller à l'ouest
        if (next_direction == 'W')
        {
            // on tourne à droite
            next_move = 'R';
        }
        // et qu'on doit aller au nord
        else if (next_direction == 'N')
        {
            // on fait demi tour
            next_move = 'U';
        }
        // et qu'on doit aller à l'est
        else if (next_direction == 'E')
        {
            // on tourne à gauche
            next_move = 'L';
        }
    }
    // si on regarde à l'ouest
    else if (lookingat == 'W')
    {
        // et qu'on doit aller au nord
        if (next_direction == 'N')
        {
            // on tourne à droite
            next_move = 'R';
        }
        // et qu'on doit aller à l'est
        else if (next_direction == 'E')
        {
            // on fait demi tour
            next_move = 'U';
        }
        // et qu'on doit aller au sud
        else if (next_direction == 'S')
        {
            // on tourne à gauche
            next_move = 'L';
        }
    }
    return next_move;    
}
