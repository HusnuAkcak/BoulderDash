#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#ifndef _BASE_H_
#define _BASE_H_

#define NAME_LENGTH 15                              /*cave name             */
#define MAP_FILE "../data/maps.txt"                    /*cave maps             */
#define FONT_SIZE 13                                /*game font size        */
#define FONT_FILE "../data/font/boulder_dash.ttf"   /*game font type        */
#define CELL_SIZE 32                                /*icon pixel            */
#define FPS 60                                      /*display frequency     */

/*PATHS..                                                                   */
#define AUDIO_PATH "../data/audio"
#define IMG_PATH "../data/img"

ALLEGRO_DISPLAY         *display;
ALLEGRO_EVENT_QUEUE     *event_queue;
ALLEGRO_TIMER           *timer;
ALLEGRO_FONT            *font;

/*Cave symbols meaning.                                                     */
typedef enum{EX_WALL='W',IN_WALL='w',SOIL='.',DIAMOND='d',ROCK='r',GATE='X',
            SPIDER='S',MINER='P',WATER='l',MONSTER='M',EMPTY_CELL=' '}Content;
typedef enum{UP, RIGHT, DOWN, LEFT}Direction;
typedef struct Point
{
    int x,y;
    struct Point*next;
}Point;

typedef struct Cave
{
    char cave_name[NAME_LENGTH];
    int dim_row,dim_col;    /*dimension_x and dimension_y       */
    int max_time,dia_req,dia_val,ex_dia_val;    /*dia =diamond  */
    Content **content;
    struct Cave *next;
}Cave;

typedef struct
{
    int score;
    int coord_r,coord_c;
    int life;
}Miner;

typedef struct Game
{
    Miner miner;
    Cave *head_cave;
}Game;


void import_caves(Game *game,char *map_file);
void free_caves(Cave *head_cave);

#endif
