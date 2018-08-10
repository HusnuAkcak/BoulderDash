#ifndef _CAVE_H_
#define _CAVE_H_
#include "base.h"

/*-- void copy_cave(...) -----------------------------------------------------
    Copies current cave from main essential record.
    This operation is performed since current cave will be manipulated during
    the game and essential records are wanted to keep untouched.
----------------------------------------------------------------------------*/
void copy_cave(Cave *dest, Cave* src);

/*-- void display_curr_cave(...) ---------------------------------------------
    Displays the current cave by calling display_cell function over and over.
----------------------------------------------------------------------------*/
void display_curr_cave(Cave * cave);

/*-- void display_cell(...) --------------------------------------------------
    Display a cell of the board, according to the content.
----------------------------------------------------------------------------*/
void display_cell(Point pos, Cave* cave);

/*-- void display_score_panel(Cave *curr_cave, Miner *m) --------------------
    Display the current score, time, and pause button at top of the screen.
---------------------------------------------------------------------------*/
void display_score_panel(Cave *curr_cave, Miner *m);

/*-- void restart_cave(...) --------------------------------------------------
    When miner dies, if he has enought life to play, current cave is restarted.
----------------------------------------------------------------------------*/
void restart_cave(Game *game,Cave *curr_cave);
#endif
