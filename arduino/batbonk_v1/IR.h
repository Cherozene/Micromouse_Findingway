// Defines IR class / functions
#include <Arduino.h>

#ifndef IR_h
#define IR_h

class IRSensors
{
    private:
        int right_pin;
        int mid_pin;
        int left_pin;
        int enable_pin;
    
    public:
        IRSensors(int right_IR_pin, int mid_IR_pin, int left_IR_pin, int enable_IR_pin);
        void enable_IR(int enable);
        void set_thresholds(int time_ms);
        void readIRs();
        void check_walls(int thres_right, int thres_left, int thres_mid);
        char closest_wall();

        // stockage de la valeur IR reçue pour chaque capteur
        int thres_right;
        int thres_mid;
        int thres_left;
        // valeurs lues au départ par les capteurs IR
        int val_right;
        int val_mid;
        int val_left;
        // flags de détection de mur
        bool wall_on_left;
        bool wall_on_right;
        bool wall_in_front;
        // mémoire dernier côté sans mur (défaut : None)
        char last_no_wall; 
};

#endif