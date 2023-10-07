// Mouse classe code
#include "mouse.h"

/* CLASSE MOUSE ET FONCTIONS ASSOCIEES */
Mouse::Mouse(int N_CELLS_Y, int N_CELLS_X)
{
    /* 
     * Inits the Mouse at position (0,0), looking North
     */
    n_cells_y = N_CELLS_Y;
    n_cells_x = N_CELLS_X; 
    y_pos = 0;
    x_pos = 0;
    lookingat = 'N';
    next_direction = 'N';
    next_move = 'F';
}

void Mouse::moveN()
{
    /* 
     * Moves mouse position one cell to the North
     */
    if (y_pos < (n_cells_y - 1))
    {
        y_pos++;
    }
    else
    {
        //cant go North
    }
}

void Mouse::moveS()
{
    /* 
     * Moves mouse position one cell to the South
     */
    if (y_pos > 0)
    {
        y_pos--;
    }
    else
    {
        //cant go South
    }
}

void Mouse::moveE()
{
    /* 
     * Moves mouse position one cell to the East
     */
    if (x_pos < (n_cells_x - 1))
    {
        x_pos++;
    }
    else
    {
        //cant go East
    }
}

void Mouse::moveW()
{
    /* 
     * Moves mouse position one cell to the West
     */
    if (x_pos > 0)
    {
        x_pos--;
    }
    else
    {
        //cant go West
    }
}

void Mouse::move_one_cell_forward(char direction)
{
    /* 
     * Updates mouse status after moving one cell forward
     */
    if (direction == 'N')
    {
        moveN();
        lookingat = 'N';
    }
    else if (direction == 'S')
    {
        moveS();
        lookingat = 'S';
    }
    else if (direction == 'E')
    {
        moveE();
        lookingat = 'E';
    }
    else if (direction == 'W')
    {
        moveW();
        lookingat = 'W';
    }
}

void Mouse::update_lookingat(char turn_direction)
{
    /* 
     * Updates the direction the mouse's looking at after turning (R right, L left, U uturn)
     */
    //char directions = 'NESW';
    if (turn_direction == 'L')
    {
        if (lookingat == 'N')
        {
            lookingat = 'W';
        }
        else if (lookingat == 'E')
        {
            lookingat = 'N';
        }
        else if (lookingat == 'S')
        {
            lookingat = 'E';
        }
        else if (lookingat == 'W')
        {
            lookingat = 'S';
        }
    }
    else if (turn_direction == 'R')
    {
        if (lookingat == 'N')
        {
            lookingat = 'E';
        }
        else if (lookingat == 'E')
        {
            lookingat = 'S';
        }
        else if (lookingat == 'S')
        {
            lookingat = 'W';
        }
        else if (lookingat == 'W')
        {
            lookingat = 'N';
        }
    }
    else if (turn_direction == 'U')
    {
        if (lookingat == 'N')
        {
            lookingat = 'S';
        }
        else if (lookingat == 'E')
        {
            lookingat = 'W';
        }
        else if (lookingat == 'S')
        {
            lookingat = 'N';
        }
        else if (lookingat == 'W')
        {
            lookingat = 'E';
        }
    }
}
