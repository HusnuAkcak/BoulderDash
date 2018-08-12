#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#ifndef _BASE_H_
#define _BASE_H_

/*###########################################################################
                                CONSTANTS
###########################################################################*/
#define NAME_LENGTH 15                              /*cave name             */
#define MAP_FILE "../data/maps.txt"                 /*cave maps             */
#define FONT_FILE "../data/font/commodore.ttf"      /*game font type        */
#define FONT_SIZE 50                                /*game font size        */
#define CELL_SIZE 64                                /*icon pixel            */
#define MINER_LIFE 3
#define FPS 60                                      /*display frequence     */
#define FALL_PS 5                                   /*falling frequence.    */
#define MPS 7                                       /*miner's speed freq.   */

/*PATHS..                                                                   */
#define AUDIO_PATH "../data/audio"
#define IMG_PATH "../data/img"
/*###########################################################################
                            END OF CONSTANTS
###########################################################################*/

/*###########################################################################
                            GLOBAL VARIABLES
###########################################################################*/
/*Allegro components.                                                       */
ALLEGRO_DISPLAY         *display;
ALLEGRO_EVENT_QUEUE     *event_queue;
ALLEGRO_TIMER           *main_timer, *panel_timer, *falling_timer, *miner_timer;
ALLEGRO_FONT            *font;
ALLEGRO_TRANSFORM       camera;

/*CAVE BITMAPS                                                              */
ALLEGRO_BITMAP *ex_wall;
ALLEGRO_BITMAP *in_wall;
ALLEGRO_BITMAP *soil;
ALLEGRO_BITMAP *diamond;
ALLEGRO_BITMAP *small_diamond;
ALLEGRO_BITMAP *rock;
ALLEGRO_BITMAP *gate;
ALLEGRO_BITMAP *spider;
ALLEGRO_BITMAP *miner;
ALLEGRO_BITMAP *miner_symbol;
ALLEGRO_BITMAP *water;
ALLEGRO_BITMAP *monster;
ALLEGRO_BITMAP *empty_cell;
ALLEGRO_BITMAP *pause_button;
ALLEGRO_BITMAP *play_button;
ALLEGRO_BITMAP *time_icon;

/*###########################################################################
                        END OF GLOBAL VARIABLES
###########################################################################*/

/*###########################################################################
                                ENUM TYPES
###########################################################################*/
/*Cave symbols meaning.                                                    */
typedef enum{EX_WALL='W',IN_WALL='w',SOIL='.',DIAMOND='d',ROCK='r',GATE='X',
            SPIDER='S',MINER='P',WATER='l',MONSTER='M',EMPTY_CELL=' '}Content;
typedef enum{UP, RIGHT, DOWN, LEFT, NONE}Direction; /*movement directions  */
typedef enum{PAUSE, RESTART, END, CONTINUE}Status;  /*status of the game   */
/*###########################################################################
                            END OF ENUM TYPES
###########################################################################*/

/*###########################################################################
                                STRUCT TYPES
###########################################################################*/
typedef struct Point{
    int c,r;/*column(c) is in place of x, row(r) is in place of y.  */
}Point;

typedef struct Cave{
    char cave_name[NAME_LENGTH];
    int dim_row,dim_col;    /*dimension_x and dimension_y           */
    int max_time,dia_req,dia_val,ex_dia_val;    /*dia =diamond      */
    Content **content;
    struct Cave *next;
}Cave;

typedef struct Miner{
    int score, curr_cave_score, collected_dia;/*current lvl collected diamond*/
    Point pos;
    int life;
}Miner;

typedef struct Game{
    Miner miner;
    Cave *head_cave;
    Point cam_pos;
    Status status;
}Game;
/*###########################################################################
                            END OF STRUCT TYPES
###########################################################################*/

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

/*-- void int_to_str(...) ---------------------------------------------------
    Converts int to char array, then assign it to dest.
---------------------------------------------------------------------------*/
void int_to_str(char *dest, int src);

/*-- void reverse_str(...) -------------------------------------------------
    Reverses given char array.
--------------------------------------------------------------------------*/
void reverse_str(char *str);

/*-- void string_cat(...) --------------------------------------------------
    merges two char arrays.
---------------------------------------------------------------------------*/
void string_cat(char* first, char *sec);

/*###########################################################################
                        END OF FUNCTION PROTOTYPES
###########################################################################*/
#endif
