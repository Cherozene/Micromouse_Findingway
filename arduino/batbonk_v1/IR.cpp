// IR class code
#include "IR.h"

IRSensors::IRSensors(int right_IR_pin, int mid_IR_pin, int left_IR_pin, int enable_IR_pin)
{
    /* 
     * Inits the IR sensors (right, mid and left + enable pin)
     */
    right_pin = right_IR_pin;
    mid_pin = mid_IR_pin;
    left_pin = left_IR_pin;
    enable_pin = enable_IR_pin;    // init IR receivers pins

    // stockage de la valeur IR reçue pour chaque capteur
    thres_right = 0;
    thres_mid = 0;
    thres_left = 0;

    // valeurs lues au départ par les capteurs IR
    val_right = 0;
    val_mid = 0;
    val_left = 0;

    // flags de détection de mur
    wall_on_left = 1;
    wall_on_right = 1;
    // mémoire dernier côté sans mur (défaut : None)
    last_no_wall = 'N'; 

    pinMode(right_pin, INPUT);
    pinMode(mid_pin, INPUT);
    pinMode(left_pin, INPUT);
    // init IR emitter pin - IR emitters are ON
    enable_IR(1);
}

void IRSensors::enable_IR(int enable)
{
    /*
     * Enable or disable the IR emitters
     * enable = 1 -> ON
     * enable = 0 -> OFF
     */
    if (enable)
    {
        digitalWrite(enable_pin, HIGH);
    }
    else
    {
        digitalWrite(enable_pin, LOW);
    }
}

void IRSensors::set_thresholds(int time_ms)
{
    /*
     * Calculates the average IR values during time_ms
     * to set the thresholds
     * init duration is time_ms (in milliseconds)
     */
    
    int time_loop_ms = 20; // loop pause in milliseconds
    int n_loops = (int)(time_ms / time_loop_ms);
    int count = 0;
    
    // 
    while (count < n_loops)
    {
        thres_right += analogRead(right_pin);
        thres_mid += analogRead(mid_pin);
        thres_left += analogRead(left_pin);
        count++;
        delay(20);
    }
    thres_right = thres_right / count;
    thres_mid = thres_mid / count;
    thres_left = thres_left / count;
}

void IRSensors::readIRs()
{
    /*
     * Reads the three IR receivers values
     */
    val_right = analogRead(right_pin);
    val_mid = analogRead(mid_pin);
    val_left = analogRead(left_pin);
}

void IRSensors::check_walls(int thres_right, int thres_left, int thres_mid)
{
    /*
     * Check if there are walls or not (left & right)
     * Params are the threshold (if < thres, there is no wall)
     * TODO: calculer ces params de manière dynamique au moment du IRSensors::set_thresholds
     */
    if (val_right < thres_right)//0.9 * IR_thres_right)
    {
        wall_on_right = 0;
        last_no_wall = 'R';
    }
    else
    {
        wall_on_right = 1;
    }
    if (val_left < thres_left)//0.9 * IR_thres_left)
    {
        wall_on_left = 0;
        last_no_wall = 'L'; 
        // priorité donnée à l'absence de mur à gauche
        // TODO: gérer ça autrement :-)
    }
    else
    {
        wall_on_left = 1;
    }
    if (wall_on_right && wall_on_left)
    {
        last_no_wall = 'N';
    }

    if (val_mid < thres_mid)//0.9 * IR_thres_mid)
    {
        wall_in_front = 0;
    }
    else
    {
        wall_in_front = 1;
    }
    
}

char IRSensors::closest_wall()
{
    /*
     * Check which sidewall is the closest (according to initial measures)
     */
    float right_IR_ratio = 0;
    float left_IR_ratio = 0;
    char side = 'L'; // if both IR are the same, returns left
    
    right_IR_ratio = val_right / thres_right;
    left_IR_ratio = val_left / thres_left;

    // si on est + proche du côté gauche qu'au départ
    if ( (left_IR_ratio < 1) && (right_IR_ratio > left_IR_ratio) ) 
    {
        side = 'R';
    }

    return side;
}


