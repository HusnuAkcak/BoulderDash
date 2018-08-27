#ifndef _OVERHEAD_H_
#define _OVERHEAD_H_
#include "base.h"

/*-- void control_falling(...) ----------------------------------------------
According to the rules of the game if there is an empty_cell under
a diamond or a rock, the object must be falled.
---------------------------------------------------------------------------*/
void control_falling(Miner *m, Cave *curr_cave);

/*-- void control_crushed_insects(...) --------------------------------------
Controls if any spider or monster under a falling rock.
---------------------------------------------------------------------------*/
void control_crushed_insects(Cave *cave, Point rock_pos);

/*-- void find_available_cells_for_dia(...) ---------------------------------
    Find available cells(empty or soil) to locate created diamonds.
---------------------------------------------------------------------------*/
void find_available_cells_for_dia(Cave *curr_cave, Point [], int arr_size, Point insect_pos);

/*-- void fill_available_cells_with_dia(...) ---------------------------------
    Fill available cells with diamond. To do that content of the map is updated.
----------------------------------------------------------------------------*/
void fill_available_cells_with_dia(Cave *curr_cave, Point [], int arr_size);

/*-- void explode_cells(...) ------------------------------------------------
    Makes cells empty.
---------------------------------------------------------------------------*/
void explode_cells(Cave *curr_cave, Point [], int arr_size);

/*-- void detect_target(...) ------------------------------------------------
Before each move 'target cell', 'after target cell'(the second cell in the
movement directions), target positon, after target position is detected.
---------------------------------------------------------------------------*/
void detect_target(Cave *cave, Miner *m,char *target, char *after_target,
    Point *target_p,Point *after_target_p);

/*-- void is_miner_dead(...) ------------------------------------------------
    Tells, if miner is died or not in this cell.
---------------------------------------------------------------------------*/
void is_miner_dead(Game *g, Cave *curr_cave, Miner *m);


/*-- void find_miner_loc(...) -----------------------------------------------
    When cave start or restart, finds miner's location from the current cave.
---------------------------------------------------------------------------*/
void find_miner_loc(Cave *curr_cave, Miner *miner);

/*-- void find_gate_loc(...) -----------------------------------------------
    Current cave's gate location is determined.
--------------------------------------------------------------------------*/
void find_gate_loc(Cave *curr_cave);

/*-- void set_monitor(...) --------------------------------------------------
    Screen view is set, according to miner's location.
---------------------------------------------------------------------------*/
void set_camera(Game *g, Cave *curr_cave);

/*-- int count_soil_cell(...) ----------------------------------------------
    Count soil cells in the cave.
---------------------------------------------------------------------------*/
int count_soil_cell_in_screen(Point start_loc, Point end_loc, Cave *cave);


/*-- int count_empty_cell(...) ----------------------------------------------
    Count empty cells in the cave.
---------------------------------------------------------------------------*/
int count_empty_cell_in_screen(Point start_loc, Point end_loc, Cave *cave);
#endif
