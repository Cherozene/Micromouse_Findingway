/* 
 * @file batbonk_v1.ino
 *
 * @brief main code for BATBONK v1.0
 * 08.2023
 *
 */
#include "SoftwareSerial.h"

#include "motor.h"
#include "PID.h"
#include "IR.h"
#include "motion.h"
#include "maze.h"
#include "mouse.h"

/* déclaration des pins */
// pins IR
#define IR_enable 4
#define IR_pin_right A5
#define IR_pin_mid A6
#define IR_pin_left A7

// pins moteurs 
#define PWM_right 10
#define forward_right 6
#define backward_right 7
#define PWM_left 11
#define forward_left 9
#define backward_left 8
// pins encodeurs moteurs (ENCA_xxxx = interruption)
#define ENCA_right 2
#define ENCA_left 3
#define ENCB_right A0
#define ENCB_left A1

/* déclaration des variables */ 

// gain appliqué au threshold pour déterminer quand il y a un mur en face
float gain_thres_mid = 3;
// seuils sous lesquels on considère l'absence d'un mur
int IR_thres_missing_right_wall = 410;
int IR_thres_missing_mid_wall = 300;
int IR_thres_missing_left_wall = 410;

// params des PID IR/moteurs
float Kp = 0.4;
float Ki = 0.03;
float Kd = 0.2;
float Kaw = Ki;
int command_sat = 20;

// vitesse de base des moteurs
unsigned int basespeed = 80; // de 0 à 255
float speed_gain_right = 1.0;
float speed_gain_left = 1.0;
int speed_correction_right = 0;
int speed_correction_left = 0;
unsigned int speed_right = 0;
unsigned int speed_left = 0;
// compteurs des encodeurs
volatile int encoder_right_counter = 0;
volatile int encoder_left_counter = 0;

// define and init motors
Motor MotorRight(PWM_right, forward_right, backward_right);
Motor MotorLeft(PWM_left, forward_left, backward_left);

// define and init PIDs 
PID PID_IR_right(Kp, Ki, Kd, Kaw, command_sat);
PID PID_IR_left(Kp, Ki, Kd, Kaw, command_sat);

// define and init IR sensors
IRSensors IR(IR_pin_right, IR_pin_mid, IR_pin_left, IR_enable);

// init an empty Maze (outer walls and cells distances are initialized)
int OBJ_Y = 3; // position Y de l'objectif
int OBJ_X = 3; // position X de l'objectif
int N_CELLS_Y = 4; // nombre de cellules en Y
int N_CELLS_X = 4; // nombre de cellules en X
Maze knownMaze(N_CELLS_Y, N_CELLS_X, OBJ_Y, OBJ_X);
// init the Mouse at position (0,0), looking North
Mouse mouse(N_CELLS_Y, N_CELLS_X);
int searching_mode = 1;
//SoftwareSerial mySerial(12, 13); // broches TX, RX
  
void setup()
{
    // init Serial (to be removed)
    // mySerial.begin(9600);
    // mySerial.println("init serial");
    // mySerial.stopListening();

    // init the IR thresholds (right/mid/left) then used as PID setpoints
    IR.set_thresholds(300);
    
    ///////////////////////
    // SETUP MOTOR ENCODERS
    ///////////////////////
    pinMode(ENCA_right, INPUT);
    pinMode(ENCB_right, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENCA_right), encoder_right_increment, RISING);
    encoder_right_counter = 0;
    pinMode(ENCA_left, INPUT);
    pinMode(ENCB_left, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENCA_left), encoder_left_increment, RISING);
    encoder_left_counter = 0;
    
    ///////////////////////
    // SETUP PIDs setpoints
    ///////////////////////
    // set the setpoints of PIDs according to the mean startup value IRs read
    PID_IR_right.set_setpoint(IR.thres_right);
    PID_IR_left.set_setpoint(IR.thres_left);
}


void loop()
{
    while (searching_mode)
    {
        // read IRs receivers
        IR.readIRs();

        // avant d'être trop proche du mur d'en face, on check les murs de la case actuelle
        //if (IR.val_mid < (2.3*IR.thres_mid)) 
        // quand on a parcouru 2/3 d'une cellule on check les infos de la prochaine
        if ( (MM_PER_SUM_COUNT * (encoder_right_counter + encoder_left_counter)) < 140)
        {
            IR.check_walls(IR_thres_missing_right_wall, IR_thres_missing_left_wall, IR_thres_missing_mid_wall);
        }

        // on compte les pas d'encodeurs pour savoir quand on a parcouru 1 case
        if ( (MM_PER_SUM_COUNT * (encoder_right_counter + encoder_left_counter)) > 180) // si on a parcouru 180mm soit une case
        {
            // motors stop
            stop_motors(MotorRight, MotorLeft);
            delay(500); // on attend 500ms sur place avant de continuer d'avancer pour vérifier qu'on a bien compté une case (temporaire)
            
            // on actualise la position de la souris
            mouse.move_one_cell_forward(mouse.lookingat);
            // on actualise les valeurs des murs (valeurs vues quand on était à 2*IR.thres_mid)
            knownMaze.update_cell_walls(mouse.y_pos, mouse.x_pos, mouse.lookingat, 
                                        IR.wall_on_right, IR.wall_on_left, IR.wall_in_front);
            // tant qu'on n'est pas arrivé sur l'objectif
            if (!((mouse.y_pos == OBJ_Y) && (mouse.x_pos == OBJ_X)))
            {
                // on actualise la valeur de la case (distance) (sauf si on est sur l'objectif)
                // et on récupère la direction du plus petit voisin accessible
                mouse.next_direction = knownMaze.update_cell_value(mouse.y_pos, mouse.x_pos);
                // on détermine le move à réaliser pour aller sur le plus petit voisin accessible (Right, Left, U-turn, Forward)
                mouse.next_move = define_next_move(mouse.lookingat, mouse.next_direction);
                // si le prochain move est un virage, on se rapproche du mur d'en face avant de tourner
                if (mouse.next_move != 'F')
                {
                    encoder_right_counter = 0;
                    encoder_left_counter = 0;
                    // micro move en avant pour pouvoir tourner comme il faut
                    move_forward(MotorRight, MotorLeft, (int)(speed_gain_right * speed_right), (int)(speed_gain_left * speed_left));
                    if (IR.wall_in_front)
                    { // s'il y a une mur devant on s'avance jusqu'au mur
                        while (IR.val_mid < (gain_thres_mid*IR.thres_mid))
                        {
                            IR.readIRs();
                        }
                    }
                    else
                    { // s'il n'y a pas de mur devant, le micro move = quelques pas encodeurs
                        while ( (MM_PER_SUM_COUNT * (encoder_left_counter + encoder_right_counter)) < 20)
                        {
                            delay(1);
                        }
                    }
                    stop_motors(MotorRight, MotorLeft);
                    encoder_right_counter = 0;
                    encoder_left_counter = 0;
                }
                // on réalise le move permettant d'aller sur le plus petit voisin accessible
                if (mouse.next_move == 'L')
                {
                    IR.last_no_wall = 'N'; // reset last missing wall data // TODO : virer cette variable désormais inutile
                    // turns 90 deg left 
                    turn_left_90(MotorRight, MotorLeft, 80, 80, &encoder_right_counter, &encoder_left_counter);
                    move_backward(MotorRight, MotorLeft, 80, 80);
                    delay(350);
                    // Check si mur à droite et/ou à gauche // TODO : virer cette étape useless
                    IR.check_walls(IR_thres_missing_right_wall, IR_thres_missing_left_wall, IR_thres_missing_mid_wall);
                    //forward_one_step(MotorRight, MotorLeft, 80, 80, &encoder_right_counter, &encoder_left_counter);
                    // update direction regardée par la souris selon la direction dans laquelle on a tourné
                    mouse.update_lookingat('L');
                    // en fin de virage/demi tour, on reset les erreurs des PID
                    PID_IR_right.reset();
                    PID_IR_left.reset();
                }
                else if (mouse.next_move == 'R')
                {
                    IR.last_no_wall = 'N'; // reset last missing wall data // TODO : virer cette variable désormais inutile
                    // turns 90 deg right 
                    turn_right_90(MotorRight, MotorLeft, 80, 80, &encoder_right_counter, &encoder_left_counter);
                    move_backward(MotorRight, MotorLeft, 80, 80);
                    delay(350);
                    // Check si mur à droite et/ou à gauche // TODO : virer cette étape useless
                    IR.check_walls(IR_thres_missing_right_wall, IR_thres_missing_left_wall, IR_thres_missing_mid_wall);
                    //forward_one_step(MotorRight, MotorLeft, 80, 80, &encoder_right_counter, &encoder_left_counter);
                    // update direction regardée par la souris selon la direction dans laquelle on a tourné
                    mouse.update_lookingat('R');
                    // en fin de virage/demi tour, on reset les erreurs des PID
                    PID_IR_right.reset();
                    PID_IR_left.reset();

                }
                else if (mouse.next_move == 'U')
                {
                    // turns approx 180 deg 
                    uturn(IR, MotorRight, MotorLeft, 80, 80, &encoder_right_counter, &encoder_left_counter);
                    // update direction regardée par la souris selon la direction dans laquelle on a tourné
                    mouse.update_lookingat('U');
                    // en fin de virage/demi tour, on reset les erreurs des PID
                    PID_IR_right.reset();
                    PID_IR_left.reset();
                }
            }
            // on a parcouru une case, on reset les encodeurs
            encoder_right_counter = 0;
            encoder_left_counter = 0;
        }

        // compute new correction
        speed_correction_right = PID_IR_right.compute_correction(IR.val_right);
        speed_correction_left = PID_IR_left.compute_correction(IR.val_left);
        // compute new speed for motors based on set basespeed and PIDs corrections
        // TODO: la condition sur les murs pour gérer les corrections va merder si aucun mur
        speed_right = min(basespeed
                            - (IR.wall_on_right + !IR.wall_on_left) * speed_correction_right
                            + (IR.wall_on_left + !IR.wall_on_right) * speed_correction_left, 255);
        speed_left = min(basespeed
                            + (IR.wall_on_right + !IR.wall_on_left) * speed_correction_right
                            - (IR.wall_on_left + !IR.wall_on_right) * speed_correction_left, 255);
        // set new motors speed - both motors go forward
        move_forward(MotorRight, MotorLeft, (int)(speed_gain_right * speed_right), (int)(speed_gain_left * speed_left));
        delay(50); // parce que sinon le p'tit cerveau d'la souris il est en panik (et le PID va trop vite)

        // SI ON EST ARRIVE SUR LA CASE OBJECTIF
        if ((mouse.y_pos == OBJ_Y) && (mouse.x_pos == OBJ_X))
        {
            stop_motors(MotorRight, MotorLeft);
            searching_mode = 0;
        }
    }   

    // TODO : à partir du labyrinthe actuel, calculer le plus court chemin connu
    // TODO : à partir du plus court chemin connu, revenir au point de départ
    // TODO : puis réaliser le plus court chemin de manière rapide


    SoftwareSerial mySerial(12, 13); // broches TX, RX
    mySerial.begin(9600);
    mySerial.println("init serial");
    mySerial.print("position y,x = ");
    mySerial.print(mouse.y_pos);
    mySerial.print(" ");
    mySerial.println(mouse.x_pos);
    mySerial.print("next dir & move = ");
    mySerial.print(mouse.next_direction);
    mySerial.print(" ");
    mySerial.println(mouse.next_move);
    for (int i = 0; i < N_CELLS_Y; i++)
    {
        for (int j = 0; j < N_CELLS_X; j++)
        {
            mySerial.print("Cellule : ");
            mySerial.print(i);
            mySerial.print(" ");
            mySerial.print(j);
            mySerial.println(" : ");
            mySerial.print("N");
            mySerial.print(knownMaze.cells[i][j].wallN);
            mySerial.print(" E");
            mySerial.print(knownMaze.cells[i][j].wallE);
            mySerial.print(" S");
            mySerial.print(knownMaze.cells[i][j].wallS);
            mySerial.print(" W");
            mySerial.println(knownMaze.cells[i][j].wallW);
            mySerial.print("VAL : ");
            mySerial.println(knownMaze.cells[i][j].distance);
            mySerial.println();
        }
    }  
  
}



void encoder_right_increment()
{
    int encb_val;
    encb_val = digitalRead(ENCB_right);
    if (encb_val)
    {
        encoder_right_counter--;
    }
    else
    {
        encoder_right_counter++;
    }
}

void encoder_left_increment()
{
    int encb_val;
    encb_val = digitalRead(ENCB_left);
    if (encb_val)
    {
        encoder_left_counter++;
    }
    else
    {
        encoder_left_counter--;
    }
}
