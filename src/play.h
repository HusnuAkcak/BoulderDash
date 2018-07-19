#include"allegro5/allegro.h"
#ifndef _PLAY_H_
#define _PLAY_H_

void start_game(ALLEGRO_DISPLAY *display,int scr_width,int scr_height);
void display_game(ALLEGRO_DISPLAY *display,Game * game);
void display_curr_cave(ALLEGRO_DISPLAY *display,Cave * cave);

#endif
