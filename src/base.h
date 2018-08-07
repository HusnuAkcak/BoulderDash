#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#ifndef _BASE_H_
#define _BASE_H_

#define NAME_LENGTH 15                              /*cave name             */
#define MAP_FILE "../data/maps.txt"                 /*cave maps             */
#define FONT_SIZE 13                                /*game font size        */
#define FONT_FILE "../data/font/boulder_dash.ttf"   /*game font type        */
#define CELL_SIZE 32                                /*icon pixel            */
#define MINER_LIFE 3
#define FPS 60                                      /*display frequence     */                        

/*PATHS..                                                                   */
#define AUDIO_PATH "../data/audio"
#define IMG_PATH "../data/img"

/*Allegro components.                                                       */
ALLEGRO_DISPLAY         *display;
ALLEGRO_EVENT_QUEUE     *event_queue;
ALLEGRO_TIMER           *timer;
ALLEGRO_FONT            *font;

/*CAVE BITMAPS                                                              */
ALLEGRO_BITMAP *ex_wall;
ALLEGRO_BITMAP *in_wall;
ALLEGRO_BITMAP *soil;
ALLEGRO_BITMAP *diamond;
ALLEGRO_BITMAP *rock;
ALLEGRO_BITMAP *gate;
ALLEGRO_BITMAP *spider;
ALLEGRO_BITMAP *miner;
ALLEGRO_BITMAP *water;
ALLEGRO_BITMAP *monster;
ALLEGRO_BITMAP *empty_cell;

/*###########################################################################
                                ENUM TYPES
###########################################################################*/
/*Cave symbols meaning.                                                     */
typedef enum{EX_WALL='W',IN_WALL='w',SOIL='.',DIAMOND='d',ROCK='r',GATE='X',
            SPIDER='S',MINER='P',WATER='l',MONSTER='M',EMPTY_CELL=' '}Content;
typedef enum{UP, RIGHT, DOWN, LEFT}Direction;   /*movement directions       */
typedef enum{RESTART, END, CONTINUE}Status;     /*status of the game        */

/*###########################################################################
                                STRUCT TYPES
###########################################################################*/
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
    int score, curr_cave_score;
    int coord_r,coord_c;
    int life;
}Miner;

typedef struct Game
{
    Miner miner;
    Cave *head_cave;
}Game;


/*###########################################################################
                            FUNCTION PROTOTYPES
###########################################################################*/

/*-- import_caves(...) ------------------------------------------------------
    The game's maps are recieving from a txt file, and saving related field
    of the structure of the game.
---------------------------------------------------------------------------*/
void import_caves(Game *game);

/*--free_caves(...) ---------------------------------------------------------
    The caves are freed.
---------------------------------------------------------------------------*/
void free_caves(Cave *head_cave);

/*--  void load_cave_bitmaps(...)  ------------------------------------------
    Cave icons are loaded.
---------------------------------------------------------------------------*/
void load_cave_bitmaps();

/*-- void destroy_cave_bitmaps(...) -----------------------------------------
    destroy_bitmap function is called for each cave bitmaps.
---------------------------------------------------------------------------*/
void destroy_cave_bitmaps();

/*-- string_cmp(...) --------------------------------------------------------
    retrun 0 :if two string is equal.
---------------------------------------------------------------------------*/
int string_cmp(const char * str1, const char *str2);

/*-- void string_cpy(...) ---------------------------------------------------
    Copies src to dest.
----------------------------------------------------------------------------*/
void string_cpy(char *dest, const char *src);

#endif
