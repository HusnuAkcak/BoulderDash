#include<stdio.h>
#include<stdlib.h>
#include"cave.h"
#include"overhead.h"
#include"insect.h"
#include"base.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

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
        if(game->head_cave==NULL){/*head node allocation*/
            game->head_cave=(Cave*)malloc(sizeof(Cave));
            curr_cave=game->head_cave;
            curr_cave->next=NULL;
        }
        else{
            curr_cave->next=(Cave*)malloc(sizeof(Cave));
            curr_cave=curr_cave->next;
            curr_cave->next=NULL;
        }

        fgets(line, LINE_SIZE, inp);
        sscanf(line, "no:%d", &curr_cave->cave_number);
        fgets(line, LINE_SIZE, inp);
        strcpy(curr_cave->cave_name, line);
        fgets(line, LINE_SIZE, inp);
        sscanf(line, "dim:%dx%d", &curr_cave->dim_col, &curr_cave->dim_row);
        fgets(line, LINE_SIZE, inp);
        sscanf(line, "time:%d", &curr_cave->max_time);
        fgets(line, LINE_SIZE, inp);
        sscanf(line, "dia_req:%d", &curr_cave->dia_req);
        fgets(line, LINE_SIZE, inp);
        sscanf(line, "dia_val:%d ex_dia_val:%d", &curr_cave->dia_val, &curr_cave->ex_dia_val);
        fgets(line, LINE_SIZE, inp);
        sscanf(line, "water_period:%d", &curr_cave->water_discharge_period);

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
    return;
}

void
copy_cave(Cave *dest, Cave* src){
    int r,c;

    /*Previous content array is free.                                       */
    for(r=0; r<(dest->dim_row); ++r){
        free(dest->content[r]);
        dest->content[r]=NULL;
    }
    if(dest->content!=NULL){
        free(dest->content);
        dest->content=NULL;
    }

    dest->cave_number=src->cave_number;
    string_cpy(dest->cave_name, src->cave_name);
    dest->dim_row=src->dim_row;
    dest->dim_col=src->dim_col;
    dest->max_time=src->max_time;
    dest->dia_req=src->dia_req;
    dest->dia_val=src->dia_val;
    dest->ex_dia_val=src->ex_dia_val;
    dest->water_discharge_period=src->water_discharge_period;
    dest->last_water_discharge_time=0;

    /*cave initial values.                                                  */
    dest->collected_dia=0;
    dest->left_time=dest->max_time;

    /*allocation for content array.                                         */
    dest->content=(Content**)calloc(dest->dim_row, sizeof(Content*));
    for(r=0;r<dest->dim_row; ++r){
        dest->content[r]=(Content*)calloc(dest->dim_col, sizeof(Content));
    }
    /*content is being copied from source to destination.                   */
    for(r=0;r<dest->dim_row;++r){
        for(c=0;c<dest->dim_col;++c){
            dest->content[r][c]=src->content[r][c];
        }
    }

    find_insects(dest);     //spiders and monsters are being found.
    find_gate_loc(dest);    //gate position is being found.
    dest->content[dest->gate_loc.r][dest->gate_loc.c]=EMPTY_CELL;// gate is hidden.

    /*when starting a new cave music volume is set its normal level.     */
    al_set_sample_instance_gain(background_instance, 1);
    return;
}

void
display_curr_screen(Cave * cave, Game *g){

    Point start_loc, end_loc, screen_dim, curr_cell, water_cell;
    int i;
    long curr_time;

    screen_dim.r=al_get_display_height(display);
    screen_dim.c=al_get_display_width(display);

    /*Real coordinate info is translated to cave coord info.                */
    start_loc.c=(g->cam_pos.c/CELL_SIZE);
    start_loc.r=(g->cam_pos.r/CELL_SIZE)+1;/*One line is reserved for score panel.*/
    end_loc.c=(g->cam_pos.c/CELL_SIZE)+(screen_dim.c/CELL_SIZE);
    end_loc.r=(g->cam_pos.r/CELL_SIZE)+(screen_dim.r/CELL_SIZE);

    /*Score panel is cleared and then redisplayed.                          */
    for(i=0; i<(end_loc.c); ++i){
        al_draw_bitmap(empty_cell, g->cam_pos.c+(i*CELL_SIZE), g->cam_pos.r, 0);
    }
    display_score_panel(cave, g);

    /*start and end locations are controlled with cave dimensions.          */
    if(start_loc.r<0)
        start_loc.r=0;
    if(start_loc.c<0)
        start_loc.c=0;
    if(end_loc.c>(cave->dim_col))
        end_loc.c=cave->dim_col;
    if(end_loc.r>(cave->dim_row))
        end_loc.r=cave->dim_row;

    /*  WATER DISCHARGE OPERATION                                           */
    curr_time=al_get_timer_count(panel_timer);
    if( g->status == CONTINUE &&                                //if status is active.
        (curr_time % (cave->water_discharge_period) == 0) &&    //in time
        (cave->last_water_discharge_time) < curr_time &&        //if water doesn't discharged at this time
        //if there is any available cell for water(empty cell or soil)
        (count_empty_cell_in_screen(start_loc, end_loc, cave) > 0 || count_soil_cell_in_screen(start_loc, end_loc, cave) > 0)
    ){
        /*It is tried to find an available location in the current screen as random.    */
        do{
            water_cell.r=(start_loc.r) + rand()%( (end_loc.r)-(start_loc.r)-1);
            water_cell.c=(start_loc.c) + rand()%( (end_loc.c)-(start_loc.c)-1);
        }
        while(cave->content[water_cell.r][water_cell.c]!=EMPTY_CELL &&
                        cave->content[water_cell.r][water_cell.c]!=SOIL);

        cave->content[water_cell.r][water_cell.c]=WATER;
        cave->last_water_discharge_time=curr_time;
    }

    /*In current screen, the cells are displayed.                           */
    for(curr_cell.c=start_loc.c; curr_cell.c<end_loc.c; ++curr_cell.c){
        for(curr_cell.r=start_loc.r; curr_cell.r<end_loc.r+1; ++curr_cell.r){
            display_cell(curr_cell, cave);
        }
    }

    return;
}

void
display_cell(Point pos, Cave* cave){
    Content content;

    /*'-1' eliminates '+1' from display_curr_screen.
    This manipulation for score panel.                                      */
    content=cave->content[pos.r-1][pos.c];

    /*Firstly, target cell is cleared.                                      */
    al_draw_bitmap(empty_cell, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);

    switch(content){
        case EX_WALL:
            al_draw_bitmap(ex_wall, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case IN_WALL:
            al_draw_bitmap(in_wall, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case SOIL:
            al_draw_bitmap(soil, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case DIAMOND:
            al_draw_bitmap(diamond, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case ROCK:
            al_draw_bitmap(rock, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case MINER:
            al_draw_bitmap(miner, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case DEAD_MINER:
            al_draw_bitmap(explosion, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case MONSTER:
            al_draw_bitmap(monster, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case GATE:
            if(((cave->dia_req)-(cave->collected_dia))>0)
                al_draw_bitmap(empty_cell, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            else
                al_draw_bitmap(gate, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);

            break;
        case SPIDER:
            al_draw_bitmap(spider, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case WATER:
            al_draw_bitmap(water, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case EMPTY_CELL:
            al_draw_bitmap(empty_cell, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        }
}

void
display_score_panel(Cave *curr_cave, Game *g){

    /*string version of the data to use in al_draw_text function            */
    char str_dia_req[NAME_LENGTH];
    char str_dia_val[NAME_LENGTH];
    char str_ex_dia_val[NAME_LENGTH];
    char str_collected_dia[NAME_LENGTH];
    char str_left_time[NAME_LENGTH];
    char str_score[NAME_LENGTH];
    char str_life[NAME_LENGTH];

    str_dia_req[0]=0;
    str_dia_val[0]=0;
    str_ex_dia_val[0]=0;
    str_collected_dia[0]=0;
    str_left_time[0]=0;
    str_score[0]=0;
    str_life[0]=0;


    if( ((curr_cave->dia_req)-(curr_cave->collected_dia)) >0){
        int_to_str(str_dia_req, curr_cave->dia_req);
        int_to_str(str_dia_val, curr_cave->dia_val);

        al_draw_text(font, al_map_rgb(100, 200, 100), g->cam_pos.c+(2*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_dia_req);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(3*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(5*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_dia_val);

    }else if( ((curr_cave->dia_req)-(curr_cave->collected_dia))<=0 ){
        int_to_str(str_ex_dia_val, curr_cave->ex_dia_val);

        al_draw_bitmap(small_diamond, g->cam_pos.c+(1*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(2*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(3*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(5*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_ex_dia_val);
    }
    int_to_str(str_collected_dia, g->miner.collected_dia);
    int_to_str(str_left_time, curr_cave->left_time);
    int_to_str(str_score, g->miner.score);
    al_draw_text(font, al_map_rgb(100, 200, 100), g->cam_pos.c+(8*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_collected_dia);
    al_draw_bitmap(time_icon, g->cam_pos.c+(9*CELL_SIZE), g->cam_pos.r, 0);

    if((curr_cave->max_time*(MUSIC_INCREASE_SLICE)) > (curr_cave->left_time))
        al_draw_text(font, al_map_rgb(155, 0, 0), g->cam_pos.c+(11*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_left_time);
    else
        al_draw_text(font, al_map_rgb(0, 155, 0), g->cam_pos.c+(11*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_left_time);

    al_draw_text(font, al_map_rgb(0, 150, 200), g->cam_pos.c+(14*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_score);

    int_to_str(str_life, g->miner.life);
    al_draw_bitmap(miner_symbol, g->cam_pos.c+(16*CELL_SIZE)-15, g->cam_pos.r, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(17*CELL_SIZE)+15, g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_life);

    if(g->status==CONTINUE)
        al_draw_bitmap(pause_button, g->cam_pos.c+(18*CELL_SIZE), g->cam_pos.r, 0);
    else if(g->status==PAUSE)
        al_draw_bitmap(play_button, g->cam_pos.c+(18*CELL_SIZE), g->cam_pos.r, 0);

    al_draw_bitmap(restart_level_button, g->cam_pos.c+(19*CELL_SIZE), g->cam_pos.r, 0);
    return;
}

void
restart_cave(Game *g,Cave *curr_cave){

    Cave *temp_cave;

    for(    temp_cave=g->head_cave;
            temp_cave->cave_number != curr_cave->cave_number;
            temp_cave=temp_cave->next
        );

    copy_cave(curr_cave, temp_cave);
    find_miner_loc(curr_cave, &(g->miner));
    g->miner.score=(g->miner.score)-(g->miner.curr_cave_score);
    g->miner.curr_cave_score=0;
    g->miner.collected_dia=0;
    g->miner.duration_of_death=MINER_DEATH_DURATION;
    g->status=CONTINUE;
    g->miner.alive=true;

    return;
}

Status
go_next_cave(Game *g, Cave *curr_cave){
    Cave *temp_cave;
    Status status;

    /* temp_cave and curr_cave are made equal                               */
    for(temp_cave=g->head_cave; temp_cave->cave_number!=curr_cave->cave_number; temp_cave=temp_cave->next);

    temp_cave=temp_cave->next;/* next cave is reached.                      */
    if(temp_cave==NULL){
        status=END;
    }
    else{
        copy_cave(curr_cave, temp_cave);
        find_miner_loc(curr_cave, &(g->miner));

        ++(g->miner.life);/*miner's life increment by one as reward for next level   */

        /*values about current level                                        */
        g->miner.curr_cave_score=0;
        g->miner.collected_dia=0;

        status=CONTINUE;
    }

    return status;
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
                curr_cave->content[i]=NULL;
            }
            free(curr_cave->content);
            curr_cave->content=NULL;
        }
        temp_cave=curr_cave;
        curr_cave=curr_cave->next;
        free(temp_cave);
        temp_cave=NULL;
    }
    return;
}
