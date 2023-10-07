// Defines Maze class
#include <math.h> 

#ifndef MAZE_h
#define MAZE_h

class Cell
{
    private:    
    public:
        Cell();
        void init_position(int POS_Y, int POS_X);
        void init_distance(int OBJ_Y, int OBJ_X);
        int pos_y;
        int pos_x; 
        bool wallN;
        bool wallS;
        bool wallW;
        bool wallE;
        int distance; // distance au but
};

class Maze
{
    private:
        int n_cells_y;
        int n_cells_x; 
        int obj_y;
        int obj_x;
        int queue_y_pos[100];
        int queue_x_pos[100];
    
    public:
        Maze(int N_CELLS_Y, int N_CELLS_X, int OBJ_Y, int OBJ_X);
        void update_cell_walls(int cell_y_pos, int cell_x_pos, char lookingat,
                               bool wall_on_right, bool wall_on_left, bool wall_in_front);   
        void check_if_accessible_neighbour_smaller(int cell_pos_y, int cell_pos_x,
                                                   int* min_neighb_dist, char* next_dir);               
        char update_cell_value(int cell_y_pos, int cell_x_pos);
        Cell cells[4][4];
};

#endif