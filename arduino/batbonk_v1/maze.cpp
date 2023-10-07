// Maze and Cells classes code
#include "maze.h"

/* CLASSE CELL ET FONCTIONS ASSOCIEES */
Cell::Cell()
{
    /* 
     * Inits a Cell (with no walls)
     */
    wallN = 0;
    wallS = 0;
    wallW = 0;
    wallE = 0;
}

void Cell::init_position(int POS_Y, int POS_X)
{
    /* 
     * Inits and set a cell's (x,y) position
     */
    pos_y = POS_Y;
    pos_x = POS_X;
}

void Cell::init_distance(int OBJ_Y, int OBJ_X)
{
    /* 
     * Inits a cell's distance to goal (with no known maze's walls)
     */
    distance = fabs(OBJ_Y - pos_y) + fabs(OBJ_X - pos_x);
}

/* CLASSE MAZE ET FONCTIONS ASSOCIEES */
Maze::Maze(int N_CELLS_Y, int N_CELLS_X, int OBJ_Y, int OBJ_X)
{
    /* 
     * Inits an empty maze (only outer walls) of size N_CELLS_Y x N_CELLS_X 
     */
    n_cells_y = N_CELLS_Y;
    n_cells_x = N_CELLS_X;
    obj_y = OBJ_Y;
    obj_x = OBJ_X;

    for(int i = 0; i < N_CELLS_Y; i++)
    {
        for(int j = 0; j < N_CELLS_X; j++)
        {
            cells[i][j].init_position(i, j);
            cells[i][j].init_distance(OBJ_Y, OBJ_X);
            if (i == 0)
            {
                cells[i][j].wallS = 1;
                if (j == 0) // en 0,0 on sait qu'on a 3 murs autour de nous
                {
                    cells[i][j].wallE = 1;
                }
            }
            if (i == (N_CELLS_Y - 1))
            {
                cells[i][j].wallN = 1;
            }
            if (j == 0)
            {
                cells[i][j].wallW = 1;
            }
            if (j == (N_CELLS_X -1))
            {
                cells[i][j].wallE = 1;
            }
        }
    }
}

void Maze::update_cell_walls(int cell_y_pos, int cell_x_pos, char lookingat,
                            bool wall_on_right, bool wall_on_left, bool wall_in_front)
{
    /* 
     * Updates known walls of the cell (cell_y_pos, cell_x_pos)
     */
    if (lookingat == 'N')
    {
        cells[cell_y_pos][cell_x_pos].wallS = 0;
    }
    else if (lookingat == 'E')
    {
        cells[cell_y_pos][cell_x_pos].wallW = 0;
    }
    else if (lookingat == 'S')
    {
        cells[cell_y_pos][cell_x_pos].wallN = 0;
    }
    else if (lookingat == 'W')
    {
        cells[cell_y_pos][cell_x_pos].wallE = 0;
    }

    // si mur à droite, selon où on regarde, on actualise le statut du mur concerné
    if (wall_on_right)
    {
        if (lookingat == 'N')
        {
            cells[cell_y_pos][cell_x_pos].wallE = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos + 1) < n_cells_x)
            {
                cells[cell_y_pos][cell_x_pos + 1].wallW = 1;
            }
        }
        else if (lookingat == 'E')
        {
            cells[cell_y_pos][cell_x_pos].wallS = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos - 1) >= 0)
            {
                cells[cell_y_pos - 1][cell_x_pos].wallN = 1;
            }
        }
        else if (lookingat == 'S')
        {
            cells[cell_y_pos][cell_x_pos].wallW = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos - 1) >= 0)
            {
                cells[cell_y_pos][cell_x_pos - 1].wallE = 1;
            }
        }
        else if (lookingat == 'W')
        {
            cells[cell_y_pos][cell_x_pos].wallN = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos + 1) < n_cells_y)
            {
                cells[cell_y_pos + 1][cell_x_pos].wallS = 1;
            }
        }
    }
    // si mur à gauche, selon où on regarde, on actualise le statut du mur concerné
    if (wall_on_left)
    {
        if (lookingat == 'N')
        {
            cells[cell_y_pos][cell_x_pos].wallW = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos - 1) >= 0)
            {
                cells[cell_y_pos][cell_x_pos - 1].wallE = 1;
            }
        }
        else if (lookingat == 'E')
        {
            cells[cell_y_pos][cell_x_pos].wallN = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos + 1) < n_cells_y)
            {
                cells[cell_y_pos + 1][cell_x_pos].wallS = 1;
            }
        }
        else if (lookingat == 'S')
        {
            cells[cell_y_pos][cell_x_pos].wallE = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos + 1) < n_cells_x)
            {
                cells[cell_y_pos][cell_x_pos + 1].wallW = 1;
            }
        }
        else if (lookingat == 'W')
        {
            cells[cell_y_pos][cell_x_pos].wallS = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos - 1) >= 0)
            {
                cells[cell_y_pos - 1][cell_x_pos].wallN = 1;
            }
        }
    }
    if (wall_in_front)
    {
        if (lookingat == 'N')
        {
            cells[cell_y_pos][cell_x_pos].wallN = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos + 1) < n_cells_y)
            {
                cells[cell_y_pos + 1][cell_x_pos].wallS = 1;
            }
        }
        else if (lookingat == 'E')
        {
            cells[cell_y_pos][cell_x_pos].wallE = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos + 1) < n_cells_x)
            {
                cells[cell_y_pos][cell_x_pos + 1].wallW = 1;
            }
        }
        else if (lookingat == 'S')
        {
            cells[cell_y_pos][cell_x_pos].wallS = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_y_pos - 1) >= 0)
            {
                cells[cell_y_pos - 1][cell_x_pos].wallN = 1;
            }
        }
        else if (lookingat == 'W')
        {
            cells[cell_y_pos][cell_x_pos].wallW = 1;
            // on actualise aussi l'autre côté du même mur (pour la cellule adjacente, si existante)
            if ((cell_x_pos - 1) >= 0)
            {
                cells[cell_y_pos][cell_x_pos - 1].wallE = 1;
            }
        }
    }

}
 
void Maze::check_if_accessible_neighbour_smaller(int cell_pos_y, int cell_pos_x,
                                                 int* min_neighb_dist, char* next_dir)
{  
    /* 
     * Check if there is a smaller neighbour (than current cell value)
     * Gives the min neighbour value and its direction from the current cell
     */
    int neighbour_distance = 255;
    // init de la distance à une valeur très grande (max théorique) (pire cas, jamais atteint)
    *min_neighb_dist = 255; 

    // si y a pas de mur au nord
    if (!(cells[cell_pos_y][cell_pos_x].wallN))
    {
        neighbour_distance = cells[cell_pos_y + 1][cell_pos_x].distance;
        // si cette case est le minimum temporaire
        if (neighbour_distance < *min_neighb_dist)
        {
            *min_neighb_dist = neighbour_distance;
            *next_dir = 'N';
        }
    }
    // si y a pas de mur à l'ouest
    if (!(cells[cell_pos_y][cell_pos_x].wallW))
    {
        neighbour_distance = cells[cell_pos_y][cell_pos_x - 1].distance;
        // si cette case est le minimum temporaire
        if (neighbour_distance < *min_neighb_dist)
        {
            *min_neighb_dist = neighbour_distance;
            *next_dir = 'W';
        }
    }
    // si y a pas de mur à l'est
    if (!(cells[cell_pos_y][cell_pos_x].wallE))
    {
        neighbour_distance = cells[cell_pos_y][cell_pos_x + 1].distance;
        // si cette case est le minimum temporaire
        if (neighbour_distance < *min_neighb_dist)
        {
            *min_neighb_dist = neighbour_distance;
            *next_dir = 'E';
        }
    }
    // si y a pas de mur au sud
    if (!(cells[cell_pos_y][cell_pos_x].wallS))
    {
        neighbour_distance = cells[cell_pos_y - 1][cell_pos_x].distance;
        // si cette case est le minimum temporaire
        if (neighbour_distance < *min_neighb_dist)
        {
            *min_neighb_dist = neighbour_distance;
            *next_dir = 'S';
        }
    }

}

char Maze::update_cell_value(int cell_y_pos, int cell_x_pos)
{
    /* 
     * Updates the cell value (distance to the goal) performing floodfill
     * Returns the next direction the mouse should go (neighbour with the smallest distance to goal)
     * Direction is a char 'N', 'E', 'S' or 'W'
     */
    int current_distance;
    int min_neighbour_distance;
    char next_dir;
    int queue_len = 0;
    int queue_position = 0;
    int curr_pos_y;
    int curr_pos_x;
    int dummy;

    queue_y_pos[queue_position] = cell_y_pos;
    queue_x_pos[queue_position] = cell_x_pos;
    queue_len++;
    queue_position = 0;

    while (queue_len != 0)
    {
        curr_pos_y = queue_y_pos[queue_position];
        curr_pos_x = queue_x_pos[queue_position];
        queue_len--;
        queue_position++;

        current_distance = cells[curr_pos_y][curr_pos_x].distance;
        check_if_accessible_neighbour_smaller(curr_pos_y, curr_pos_x, &min_neighbour_distance, &next_dir);
        if (min_neighbour_distance > current_distance)
        {
            // on actualise la valeur de la cellule actuelle
            cells[curr_pos_y][curr_pos_x].distance = min_neighbour_distance + 1;
            // on ajoute ses voisins accessibles à la queue
            if (!(cells[curr_pos_y][curr_pos_x].wallN))
            {
                // ajoute à la queue la cellule nord
                queue_y_pos[queue_position + queue_len] = curr_pos_y + 1;
                queue_x_pos[queue_position + queue_len] = curr_pos_x;
                queue_len++;
            }
            if (!(cells[curr_pos_y][curr_pos_x].wallS))
            {
                // ajoute à la queue la cellule sud
                queue_y_pos[queue_position + queue_len] = curr_pos_y - 1;
                queue_x_pos[queue_position + queue_len] = curr_pos_x;
                queue_len++;
            }
            if (!(cells[curr_pos_y][curr_pos_x].wallE))
            {
                // ajoute à la queue la cellule est
                queue_y_pos[queue_position + queue_len] = curr_pos_y;
                queue_x_pos[queue_position + queue_len] = curr_pos_x + 1;
                queue_len++;
            }
            if (!(cells[curr_pos_y][curr_pos_x].wallW))
            {
                // ajoute à la queue la cellule ouest
                queue_y_pos[queue_position + queue_len] = curr_pos_y;
                queue_x_pos[queue_position + queue_len] = curr_pos_x - 1;
                queue_len++;
            }
        }
    }
    check_if_accessible_neighbour_smaller(cell_y_pos, cell_x_pos, &dummy, &next_dir);

    return next_dir;

}







