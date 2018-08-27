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

/*-- void calc_monster_route(...) ----------------------------------------
    Calculates the shortest way to catch miner.
-------------------------------------------------------------------------*/
void calc_monster_route(Point miner_pos, Cave *curr_cave, Monster *curr_monster, Point *curr_route, int curr_route_size, Direction move_dir, int *shortest_distance);

/*-- Direction can_spider_move(...) ----------------------------------------
    Returns true if a spider can move one cell from given pos to given dir.
--------------------------------------------------------------------------*/
Direction can_spider_move(Cave *cave, Point pos, Direction dir);

/*-- void move_insects(...) ------------------------------------------------
    Take insects linked list and calls required function according to insect type.
--------------------------------------------------------------------------*/
void move_insects(Game *g,Cave *curr_cave);

/*-- void move_monsters(...) ------------------------------------------------
    According to the rules, updates the monster location.
---------------------------------------------------------------------------*/
void move_monster(Point miner_pos, Cave *curr_cave, Monster *curr_monster);

/*-- void move_monsters(...) ------------------------------------------------
    According to the rules, updatesthe  spider location.
---------------------------------------------------------------------------*/
void move_spider(Cave *curr_cave, Spider *curr_spider);

/*-- bool is_miner_caught(...) ----------------------------------------------
    Tells, if the miner caught or not.
---------------------------------------------------------------------------*/
bool is_miner_caught(Point miner_pos, Point pos);

/*-- bool monster_move_possible(...) ---------------------------------------
    Tells, if monster can move or not.
--------------------------------------------------------------------------*/
bool monster_move_possible(Cave *cave, Point *curr_route, int curr_route_size, Point pos, Direction dir);

/*-- void apply_monster_next_move(...) ------------------------------------
    Applies the monster's next move to the current cave.
-------------------------------------------------------------------------*/
void apply_monster_next_move(Cave *cave, Monster *monster);

#endif
