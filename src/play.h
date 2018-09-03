#ifndef _PLAY_H_
#define _PLAY_H_
#include "base.h"

/*-- void intro_game(...) ---------------------------------------------------
        Starts the game with intro images and sounds.
---------------------------------------------------------------------------*/
void intro_game(Game *game,int scr_width,int scr_height);

/*-- void play_game(...) --------------------------------------------------
    Display and confugure the game.
----------------------------------------------------------------------------*/
void play_game(Game * game);

/*-- Status move(...) ---------------------------------------------------------
    When player wants to make a move this function is called.
----------------------------------------------------------------------------*/
Status move(Cave *curr_cave,Miner *m);

/*-- void show_final_results(...) --------------------------------------------
    Shows final results to the user on final screen.
----------------------------------------------------------------------------*/
void show_final_results(Game *g ,Cave *curr_cave);
#endif
