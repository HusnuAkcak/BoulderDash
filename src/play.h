#ifndef _PLAY_H_
#define _PLAY_H_

void start_game(Game *game,int scr_width,int scr_height);
void display_game(Game * game);
void display_curr_cave(Cave * cave);

/*############################################################
    When player wants to make a move this function is called.
    Return values:
        true    :if the game is continued.
        false   :if the game is terminated.
############################################################*/
bool move(Cave *curr_cave,Miner *m,Direction dir);

/*###########################################################
    Display a cell of the board, according to the content.
###########################################################*/
void display_cell(int row, int column,Content content);
#endif
