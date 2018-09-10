#include <stdio.h>
#include "base.h"

void
test_import(Game *game)
{
    int cave_num,i,j;
    Cave *curr_cave;
    char *map_file_name;
    FILE *out_p;

    map_file_name="../test_map.txt";
    if(game->head_cave==NULL)
    {
        printf("There is no cave to test.\n");
        return;
    }
    else
    {
        printf("Initialization is being tested...\n");
    }

    out_p=fopen(map_file_name,"w");
    cave_num=0;
    for(curr_cave=game->head_cave;curr_cave->next!=NULL;curr_cave=curr_cave->next)
    {
        ++cave_num;
        fprintf(out_p,"Cave No         :%d\n",curr_cave->cave_number);
        fprintf(out_p,"Cave Name       :%s",curr_cave->cave_name);
        fprintf(out_p,"Cave Dimension  :%d %d\n",curr_cave->dim_row,curr_cave->dim_col);
        fprintf(out_p,"Time            :%d\n",curr_cave->max_time);
        fprintf(out_p,"Diamond Req.    :%d\n",curr_cave->dia_req);
        fprintf(out_p,"Diamond Val.    :%d %d\n",curr_cave->dia_val,curr_cave->ex_dia_val);
        fprintf(out_p,"Water Period    :%d\n",curr_cave->water_discharge_period);

        for(i=0; i<(curr_cave->dim_row); ++i)
        {
            for(j=0; j<(curr_cave->dim_col); ++j)
            {
                fprintf(out_p,"%c",curr_cave->content[i][j]);
            }
            fprintf(out_p,"\n");
        }
        fprintf(out_p,"\n\n");
    }

    printf("The test is complated.\n");
    printf("The caves are exported to %s\n",map_file_name);
    fclose(out_p);

    return;
}
