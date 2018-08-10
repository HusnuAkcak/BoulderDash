#ifndef _OVERHEAD_H_
#define _OVERHEAD_H_
#include "base.h"

/*-- bool is_miner_dead(...) ------------------------------------------------
    Tells, if miner is died or not in this cell.
---------------------------------------------------------------------------*/
bool is_miner_dead(Cave *curr_cave, Miner *m);

/*-- void control_falling(...) ----------------------------------------------
    According to the rules of the game if there is an empty_cell under
    a diamond or a rock, the object must be falled.
---------------------------------------------------------------------------*/
void control_falling(Cave *curr_cave);

/*-- void find_miner_loc(...) -----------------------------------------------
    When cave start or restart, finds miner's location from the current cave.
---------------------------------------------------------------------------*/
void find_miner_loc(Cave *curr_cave, Miner *miner);

/*-- void detect_target(...) ------------------------------------------------
    Before each move 'target cell', 'after target cell'(the second cell in the
movement directions), target positon, after target position is detected.
---------------------------------------------------------------------------*/
void detect_target(Direction dir, Cave *cave, Miner *m,
    char *target, char *after_target, Point *target_p,Point *after_target_p);

#endif
