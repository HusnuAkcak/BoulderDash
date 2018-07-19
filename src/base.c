#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"
#include"base.h"

#define LINE_SIZE 240   /*when it is changes, update 'README' and 'maps.txt'
                    file to specify the line size requirement.              */


void
import_caves(Game *game,char *map_file)
{
    FILE *inp;
    char line[LINE_SIZE];
    Cave *curr_cave;
    int i,j;

    inp=fopen("../maps.txt","r");


    if(game->head_cave==NULL)
    {
        game->head_cave=(Cave*)malloc(sizeof(Cave));
        curr_cave=game->head_cave;
    }

    /*the lines are ignored, until the first [cave] is read.            */
    while(strcmp(line,"[cave]\n")!=0)
    {
        fgets(line,LINE_SIZE,inp);
    }
    /*until the game finishes...*/
    while((inp!=NULL)&&(strcmp(line,"[/game]\n")!=0))
    {
        fgets(line,LINE_SIZE,inp);
        strcpy(curr_cave->cave_name,line);
        fgets(line,LINE_SIZE,inp);
        sscanf(line,"%dx%d",&curr_cave->dim_col,&curr_cave->dim_row);
        fgets(line,LINE_SIZE,inp);
        sscanf(line,"%d",&curr_cave->max_time);
        fgets(line,LINE_SIZE,inp);
        sscanf(line,"%d",&curr_cave->dia_req);
        fgets(line,LINE_SIZE,inp);
        sscanf(line,"%d %d",&curr_cave->dia_val,&curr_cave->ex_dia_val);

        /*the lines are ignored until map is read.                         */
        while(strcmp(line,"[map]\n")!=0)
        {
            fgets(line,LINE_SIZE,inp);
        }
        /*the content of the map is receiving until [/map] is read         */
        /*the 'content' array is allocated.                                */
        curr_cave->content=(char**)calloc((curr_cave->dim_row),sizeof(char*));
        for(i=0;i<(curr_cave->dim_row);++i)
        {
            curr_cave->content[i]=(char*)calloc((curr_cave->dim_col),sizeof(char));
        }

        /*content of the map is read according to the dimension info.       */
        for(i=0;i<(curr_cave->dim_row);++i)
        {
            /*  +2 comes from the newline and the NULL character at the end */
            fgets(line,(curr_cave->dim_col)+2,inp);

            /*we ignored the last two characters which are newline and NULL */
            for(j=0;j<(curr_cave->dim_col);++j)
            {
                curr_cave->content[i][j]=line[j];
            }
        }

        /*after the curr cave read, the line ignored until a new cave is read
        or the game terminated.If the game is terminated loop will be break */
        while(strcmp("[cave]\n",line)!=0 && strcmp("[/game]\n",line)!=0)
        {
            fgets(line,LINE_SIZE,inp);
        }

        curr_cave->next=(Cave*)malloc(sizeof(Cave));
        curr_cave=curr_cave->next;
    }

}

void
free_caves(Cave *head_cave)
{
    Cave *curr_cave,*temp_cave;
    int i;

    curr_cave=head_cave;
    while(curr_cave!=NULL)
    {
        /*the char array that keeps content of the map                      */
        if(curr_cave->content!=NULL)
        {
            for(i=0;i>(curr_cave->dim_col);++i)
            {
                free(curr_cave->content[i]);
            }
            free(curr_cave->content);
        }

        temp_cave=curr_cave;
        curr_cave=curr_cave->next;
        free(temp_cave);

    }
}
