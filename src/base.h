#include"allegro5/allegro.h"
#ifndef _BASE_H_
#define _BASE_H_

#define NAME_LENGTH 15

typedef struct Cave
{
    char cave_name[NAME_LENGTH];
    int dim_row,dim_col;    /*dimension_x and dimension_y       */
    int max_time,dia_req,dia_val,ex_dia_val;    /*dia =diamond  */
    char **content;
    struct Cave *next;
}Cave;

typedef struct Miner
{
    int score;
    int live;
}Miner;

typedef struct Game
{
    Miner miner;
    Cave *head_cave;
}Game;


void import_caves(Game *game,char *map_file);
void free_cave(Cave *head_cave);

#endif
