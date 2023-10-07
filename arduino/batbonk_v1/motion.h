// Defines motion functions and macros

// distance parcourue pour 1 pas d'encodeur
#define MM_PER_COUNT 0.550 // 1 pas encodeur = MM_PER_COUNT millimètres parcourus
#define MM_PER_SUM_COUNT MM_PER_COUNT / 2 // si on calcule la somme des 2 encodeurs

#define ENC_COUNTS_360 (3.14159 * 85) / MM_PER_COUNT  // pi * 85mm d'espacement entre roues - nombre de counts pour faire 360 degrés
#define DEG_PER_COUNT 360 / (ENC_COUNTS_360 * 2) // nombre de degrés tournés par pas d'encodeur (2 moteurs actifs en même temps)

#ifndef MOTION_h
#define MOTION_h

void stop_motors(Motor MotorRight, Motor MotorLeft);
void move_forward(Motor MotorRight, Motor MotorLeft, unsigned int speed_right, unsigned int speed_left);
void move_backward(Motor MotorRight, Motor MotorLeft, unsigned int speed_right, unsigned int speed_left);
void turn_right_90(Motor MotorRight, Motor MotorLeft, 
                  unsigned int speed_right, unsigned int speed_left,
                  int* encoder_right_counter, int* encoder_left_counter,
                  int angle = 90);
void turn_left_90(Motor MotorRight, Motor MotorLeft, 
                  unsigned int speed_right, unsigned int speed_left,
                  int* encoder_right_counter, int* encoder_left_counter,
                  int angle = 90);

void forward_one_step(Motor MotorRight, Motor MotorLeft, 
                     unsigned int speed_right, unsigned int speed_left,
                     int* encoder_right_counter, int* encoder_left_counter);
                     
void uturn(IRSensors IR, Motor MotorRight, Motor MotorLeft, 
           unsigned int speed_right, unsigned int speed_left,
           int* encoder_right_counter, int* encoder_left_counter);

char define_next_move(char lookingat, char next_direction);

#endif