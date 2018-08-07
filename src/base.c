#include<stdio.h>
#include<stdlib.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"
#include"base.h"

#define LINE_SIZE 240   /*when it is changes, update 'README' and 'maps.txt'
                    file to specify the line size requirement.              */

void
string_cpy(char *dest, const char *src){

    int i;

    for(i=0;src[i]!=0;++i){
        dest[i]=src[i];
    }
    dest[i]=0;
    return;
}

int
string_cmp(const char * str1, const char *str2){
    int status; /*if strings are equal, status will be 0.                */
    int i;      /*loop variable                                          */

    status=0;
    i=0;
    while(status==0 && (str1[i]!=0 || str2[i]!=0)){
        if(str1[i]>str2[i]){
            status=1;
        }else if(str1[i]<str2[i]){
            status=-1;
        }
        ++i;
    }

    return status;
}

void
import_caves(Game *game){
    FILE *inp;
    char line[LINE_SIZE];
    Cave *curr_cave;
    int i,j;

    inp=fopen(MAP_FILE,"r");
    game->head_cave=NULL;
    curr_cave=NULL;

    /*the lines are ignored, until the first [cave] is read.            */
    while(string_cmp(line,"[cave]\n")!=0){
        fgets(line,LINE_SIZE,inp);
    }
    /*until the game finishes...                                        */
    while((inp!=NULL)&&(string_cmp(line,"[/game]\n")!=0)){
        if(game->head_cave==NULL){
            game->head_cave=(Cave*)malloc(sizeof(Cave));
            curr_cave=game->head_cave;
            curr_cave->next=NULL;
        }
        else{
            curr_cave->next=(Cave*)malloc(sizeof(Cave));
            curr_cave=curr_cave->next;
            curr_cave->next=NULL;
        }

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
        while(string_cmp(line,"[map]\n")!=0){
            fgets(line,LINE_SIZE,inp);
        }
        /*the content of the map is receiving until [/map] is read         */
        /*the 'content' array is allocated.                                */
        curr_cave->content=(Content**)calloc((curr_cave->dim_row),sizeof(Content*));
        for(i=0;i<(curr_cave->dim_row);++i){
            curr_cave->content[i]=(Content*)calloc((curr_cave->dim_col),sizeof(Content));
        }

        /*content of the map is read according to the dimension info.       */
        for(i=0;i<(curr_cave->dim_row);++i){
            /*  +2 comes from the newline and the NULL character at the end */
            /*we ignored the last two characters which are newline and NULL */
            fgets(line,(curr_cave->dim_col)+2,inp);
            for(j=0;j<(curr_cave->dim_col);++j){
                curr_cave->content[i][j]=line[j];
            }
        }

        /*after the curr cave read, the line ignored until a new cave is read
        or the game terminated.If the game is terminated loop will be break */
        while(string_cmp("[cave]\n",line)!=0 && string_cmp("[/game]\n",line)!=0){
            fgets(line,LINE_SIZE,inp);
        }
    }
}

void
free_caves(Cave *head_cave){
    Cave *curr_cave,*temp_cave;
    int i;
    curr_cave=head_cave;
    while(curr_cave!=NULL){
        if(curr_cave->content!=NULL){
            for(i=0;i<(curr_cave->dim_row);++i){
                free(curr_cave->content[i]);
            }
            free(curr_cave->content);
        }
        temp_cave=curr_cave;
        curr_cave=curr_cave->next;
        free(temp_cave);
    }
    return;
}

void
load_cave_bitmaps(){
    in_wall=al_load_bitmap(IMG_PATH"/in_wall.png");
    ex_wall=al_load_bitmap(IMG_PATH"/ex_wall.png");
    soil=al_load_bitmap(IMG_PATH"/soil.png");
    diamond=al_load_bitmap(IMG_PATH"/diamond.png");
    rock=al_load_bitmap(IMG_PATH"/rock.png");
    gate=al_load_bitmap(IMG_PATH"/gate.png");
    spider=al_load_bitmap(IMG_PATH"/spider.png");
    miner=al_load_bitmap(IMG_PATH"/miner.png");
    water=al_load_bitmap(IMG_PATH"/water.png");
    monster=al_load_bitmap(IMG_PATH"/monster.png");
    empty_cell=al_load_bitmap(IMG_PATH"/empty_cell.png");
}

void destroy_cave_bitmaps(){
    al_destroy_bitmap(in_wall);
    al_destroy_bitmap(ex_wall);
    al_destroy_bitmap(soil);
    al_destroy_bitmap(diamond);
    al_destroy_bitmap(rock);
    al_destroy_bitmap(gate);
    al_destroy_bitmap(spider);
    al_destroy_bitmap(miner);
    al_destroy_bitmap(water);
    al_destroy_bitmap(monster);
    al_destroy_bitmap(empty_cell);
}
