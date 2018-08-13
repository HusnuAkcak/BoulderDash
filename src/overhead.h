#ifndef _OVERHEAD_H_
#define _OVERHEAD_H_
#include "base.h"

/*-- void control_falling(...) ----------------------------------------------
According to the rules of the game if there is an empty_cell under
a diamond or a rock, the object must be falled.
---------------------------------------------------------------------------*/
void control_falling(Cave *curr_cave);

/*-- void detect_target(...) ------------------------------------------------
Before each move 'target cell', 'after target cell'(the second cell in the
movement directions), target positon, after target position is detected.
---------------------------------------------------------------------------*/
void detect_target(Direction dir, Cave *cave, Miner *m,
    char *target, char *after_target, Point *target_p,Point *after_target_p);

/*-- bool is_miner_dead(...) ------------------------------------------------
    Tells, if miner is died or not in this cell.
---------------------------------------------------------------------------*/
bool is_miner_dead(Cave *curr_cave, Miner *m);


/*-- void find_miner_loc(...) -----------------------------------------------
    When cave start or restart, finds miner's location from the current cave.
---------------------------------------------------------------------------*/
void find_miner_loc(Cave *curr_cave, Miner *miner);


/*-- void set_monitor(...) --------------------------------------------------
    Screen view is set, according to miner's location.
---------------------------------------------------------------------------*/
void set_camera(Game *g, Cave *curr_cave);

/*-- void find_insects(...) ------------------------------------------------
    Finds insects(spiders and monsters) position in the current cave.
--------------------------------------------------------------------------*/
void find_insects(Cave *curr_cave);

/*-- void calc_spiders_route(...) ------------------------------------------
    Calulates cave spiders' route.
--------------------------------------------------------------------------*/
void calc_spiders_route(Cave *curr_cave, Spider *curr_spider, Direction dir);

/*-- Direction can_spider_move(...) -----------------------------------------------
    Returns true if a spider can move one cell from given pos to given dir.
--------------------------------------------------------------------------*/
Direction can_spider_move(Cave *cave, Point pos, Direction dir);

/*-- void calc_monsters_route(...) -----------------------------------------
    Calulates cave monsters' route.
--------------------------------------------------------------------------*/
void calc_monsters_route(Cave *curr_cave, Monster *curr_monster);

/*-- void move_insects(...) ------------------------------------------------
    Take insects linked list and calls required function according to insect type.
--------------------------------------------------------------------------*/
void move_insects(Cave *curr_cave);

/*-- void move_monsters(...) ------------------------------------------------
    According to the rules, updates the monster location.
---------------------------------------------------------------------------*/
void move_monster(Cave *curr_cave, Monster *curr_monster);

/*-- void move_monsters(...) ------------------------------------------------
    According to the rules, updatesthe  spider location.
---------------------------------------------------------------------------*/
void move_spider(Cave *curr_cave, Spider *curr_spider);
#endif
