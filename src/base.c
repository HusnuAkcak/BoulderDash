#include <stdio.h>
#include "allegro5/allegro.h"
#include "import.h"


void
init_game(Game *game,char *map_file)
{
    FILE *inp;

    /*
    char *cave_name;
    int score,max_time,diamond_num,diamond_value,extra_diamond_value;
    char **content;
    */

    inp=fopen(map_file,"r");

    while(inp!=NULL)
    {
        fgets(inp,)
    }

}
