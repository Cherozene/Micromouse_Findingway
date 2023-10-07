// Defines Mouse class

#ifndef MOUSE_h
#define MOUSE_h

class Mouse
{
    private:
        void moveN();
        void moveS();
        void moveE();
        void moveW();
        int n_cells_y;
        int n_cells_x; 
    
    public:
        Mouse(int N_CELLS_Y, int N_CELLS_X);
        void move_one_cell_forward(char direction);
        void update_lookingat(char turn_direction);
        int y_pos;
        int x_pos;
        char lookingat;
        char next_direction;
        char next_move;
};

#endif