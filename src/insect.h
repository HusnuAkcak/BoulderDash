#ifndef _INSECT_H_
#define _INSECT_H_
#include "base.h"

/*-- void find_insects(...) ------------------------------------------------
    Finds insects(spiders and monsters) position in the current cave.
--------------------------------------------------------------------------*/
void find_insects(Cave *curr_cave);

/*-- void calc_spiders_route(...) ------------------------------------------
    Calulates cave spiders' route.
--------------------------------------------------------------------------*/
void calc_spiders_route(Cave *curr_cave, Spider *curr_spider, Direction dir);

/*-- Direction can_spider_move(...) ----------------------------------------
    Returns true if a spider can move one cell from given pos to given dir.
--------------------------------------------------------------------------*/
Direction can_spider_move(Cave *cave, Point pos, Direction dir);

/*-- void move_insects(...) ------------------------------------------------
    Take insects linked list and calls required function according to insect type.
--------------------------------------------------------------------------*/
void move_insects(Game *g,Cave *curr_cave);

/*-- void move_monsters(...) ------------------------------------------------
    According to the rules, updatesthe  spider location.
---------------------------------------------------------------------------*/
void move_spider(Cave *curr_cave, Spider *curr_spider);

/*-- bool is_miner_caught(...) ----------------------------------------------
    Tells, if the miner caught or not.
---------------------------------------------------------------------------*/
bool is_miner_caught(Point miner_pos, Point pos);

#endif
