#ifndef _PLAY_H_
#define _PLAY_H_

/*-- void start_game(...) ---------------------------------------------------
        Starts the game with intro images and sounds, then calls 'display_game'
    function.
---------------------------------------------------------------------------*/
void start_game(Game *game,int scr_width,int scr_height);

/*-- void display_game(...) --------------------------------------------------
    Display anc confugure the game.
----------------------------------------------------------------------------*/
void display_game(Game * game);

/*-- void copy_cave(...) -----------------------------------------------------
    Copies current cave from main essential record.
    This operation is performed since current cave will be manipulated during
    the game and essential records are wanted to keep untouched.
----------------------------------------------------------------------------*/
void copy_cave(Cave *dest, Cave* src);

/*-- void restart_cave(...) --------------------------------------------------
    When miner dies, if he has enought life to play, current cave is restarted.
----------------------------------------------------------------------------*/
void restart_cave(Game *game,Cave *curr_cave);

/*-- void display_curr_cave(...) ---------------------------------------------
    Displays the current cave by calling display_cell function over and over.
----------------------------------------------------------------------------*/
void display_curr_cave(Cave * cave);

/*-- void display_cell(...) --------------------------------------------------
    Display a cell of the board, according to the content.
----------------------------------------------------------------------------*/
void display_cell(int row, int column,Content content);

/*-- bool move(...) ---------------------------------------------------------
    When player wants to make a move this function is called.
    Return values:
        true    :if the game is continued.
        false   :if the game is terminated.
----------------------------------------------------------------------------*/
Status move(Cave *curr_cave,Miner *m,Direction dir);

/*-- bool is_miner_dead(...) ------------------------------------------------
    Tells, if miner is died or not in this cell.
---------------------------------------------------------------------------*/
bool is_miner_dead(Cave *curr_cave, Miner *m);

/*-- void display_score_panel(Cave *curr_cave, Miner *m) --------------------
    Display the current score, time, and pause button at top of the screen.
---------------------------------------------------------------------------*/
void display_score_panel(Cave *curr_cave, Miner *m);

/*-- void control_falling(...) ----------------------------------------------
    After each move it, current cave may be changed. According to the rules of
    the game if there is an empty_cell under a diamond or a rock, the object
    must be falled.
---------------------------------------------------------------------------*/
void control_falling(Cave *curr_cave, int r, int c);

/*-- void find_miner_loc(...) -----------------------------------------------
    When cave start or restart, finds miner's location from the current cave.
---------------------------------------------------------------------------*/
void find_miner_loc(Cave *curr_cave, Miner *miner);

#endif
