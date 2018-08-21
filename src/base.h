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
#define FALL_PS 3                                   /*falling frequence.    */
#define MPS 7                                       /*miner's speed freq.   */
#define DIA_OF_SPIDER 8     /*diamond number that is created after any spider is crushed. */
#define DIA_OF_MONSTER 12   /*diamond number that is created after any monster is crushed. */
#define LINE_SIZE 240   /*when it is changes, update 'README' and 'maps.txt'
                            file to specify the line size requirement.      */

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
ALLEGRO_BITMAP *explosion;

/*###########################################################################
                        END OF GLOBAL VARIABLES
###########################################################################*/

/*###########################################################################
                                ENUM TYPES
###########################################################################*/
/*Cave symbols meaning.                                                    */
typedef enum{EX_WALL='W',IN_WALL='w',SOIL='.',DIAMOND='d',ROCK='r',GATE='X',
            SPIDER='S',MINER='P',WATER='l',MONSTER='M',EMPTY_CELL=' '}Content;
typedef enum{UP, RIGHT, DOWN, LEFT, NONE}Direction; /*movement directions       */
typedef enum{PAUSE, RESTART, END, CONTINUE, NEXT}Status;/*status of the game    */
/*###########################################################################
                            END OF ENUM TYPES
###########################################################################*/

/*###########################################################################
                                STRUCT TYPES
###########################################################################*/
typedef struct Point{
    int c,r;/*column(c) is in place of x, row(r) is in place of y.  */
    struct Point *next;
}Point;

typedef struct Monster{
    Point pos;
    Point *route_head;
    struct Monster *next;
    bool alive;
}Monster;

typedef struct Spider{
    Point pos;
    Point next_pos;
    struct Spider *next;
    Direction move_dir;/*current movement direction                         */
    bool alive;
}Spider;

typedef struct Cave{
    int cave_number;
    char cave_name[NAME_LENGTH];
    int dim_row,dim_col;    /*dimension_x and dimension_y                   */
    int max_time,dia_req,collected_dia,dia_val,ex_dia_val;/*dia =diamond    */
    int water_discharge_period;
    Content **content;
    Monster *head_monster;
    Spider  *head_spider;
    struct Cave *next;
}Cave;

typedef struct Miner{
    int score, curr_cave_score, collected_dia;/*current lvl collected diamond*/
    Point pos;
    int life;
    bool alive;
    Direction move_dir;
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
