#ifndef _PLAY_H_
#define _PLAY_H_

/*---------------------------------------------------------------------------
        Starts the game with intro images and sounds, then calls 'display_game'
    function.
---------------------------------------------------------------------------*/
void start_game(Game *game,int scr_width,int scr_height);

/*----------------------------------------------------------------------------
    Display anc confugure the game.
-----------------------------------------------------------------------------*/
void display_game(Game * game);

/*----------------------------------------------------------------------------
    Displays the current cave by calling display_cell function over and over.
----------------------------------------------------------------------------*/
void display_curr_cave(Cave * cave);

/*----------------------------------------------------------------------------
    Display a cell of the board, according to the content.
----------------------------------------------------------------------------*/
void display_cell(int row, int column,Content content);

/*----------------------------------------------------------------------------
    When player wants to make a move this function is called.
    Return values:
        true    :if the game is continued.
        false   :if the game is terminated.
----------------------------------------------------------------------------*/
bool move(Cave *curr_cave,Miner *m,Direction dir);

/*---------------------------------------------------------------------------
    When cave start or restart, finds miner's location from the current cave.
---------------------------------------------------------------------------*/
void find_miner_loc(Cave *curr_cave, Miner *miner);
#endif
