#include<stdio.h>
#include<stdlib.h>
#include"cave.h"
#include"overhead.h"
#include"base.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
copy_cave(Cave *dest, Cave* src){
    int r,c;

    for(r=0;r<dest->dim_row;++r){
        free(dest->content[r]);
    }
    if(dest->content!=0){
        free(dest->content);
    }

    string_cpy(dest->cave_name, src->cave_name);
    dest->dim_row=src->dim_row;
    dest->dim_col=src->dim_col;
    dest->max_time=src->max_time;
    dest->dia_req=src->dia_req;
    dest->dia_val=src->dia_val;
    dest->ex_dia_val=src->ex_dia_val;

    dest->content=(Content**)calloc(dest->dim_row, sizeof(Content*));
    for(r=0;r<dest->dim_row; ++r){
        dest->content[r]=(Content*)calloc(dest->dim_col, sizeof(Content));
    }
    for(r=0;r<dest->dim_row;++r){
        for(c=0;c<dest->dim_col;++c){
            dest->content[r][c]=src->content[r][c];
        }
    }
    return;
}

void
display_curr_screen(Cave * cave, Game *g){

    Point start_loc, end_loc, screen_dim, curr_cell;

    display_score_panel(cave, g);

    screen_dim.r=al_get_display_height(display);
    screen_dim.c=al_get_display_width(display);

    /*Real coordinate info is translated to cave coord info.            */
    start_loc.c=(g->cam_pos.c/CELL_SIZE);
    start_loc.r=(g->cam_pos.r/CELL_SIZE)+1;/*One line is reserved for score panel.*/
    end_loc.c=(g->cam_pos.c/CELL_SIZE)+(screen_dim.c/CELL_SIZE);
    end_loc.r=(g->cam_pos.r/CELL_SIZE)+(screen_dim.r/CELL_SIZE);

    /*In border, the cells are displayed.                               */
    for(curr_cell.c=start_loc.c; curr_cell.c<end_loc.c; ++curr_cell.c){
        for(curr_cell.r=start_loc.r; curr_cell.r<end_loc.r; ++curr_cell.r){
            display_cell(curr_cell, cave);
        }
    }

    return;
}

void
display_cell(Point pos,Cave* cave){
    Content content;

    if(pos.r>0)
        content=cave->content[pos.r-1][pos.c];
    //pos.r+=1;/*first line is reserved for score panel.                       */
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
        case MONSTER:
            al_draw_bitmap(monster, (pos.c)*CELL_SIZE, ((pos.r)*CELL_SIZE), 0);
            break;
        case GATE:
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
    int i;
    /*string version of the data to use in al_draw_text function            */
    char str_dia_req[NAME_LENGTH];
    char str_dia_val[NAME_LENGTH];
    char str_ex_dia_val[NAME_LENGTH];
    char str_collected_dia[NAME_LENGTH];
    char str_max_time[NAME_LENGTH];
    char str_score[NAME_LENGTH];

    str_dia_req[0]=0;
    str_dia_val[0]=0;
    str_ex_dia_val[0]=0;
    str_collected_dia[0]=0;
    str_max_time[0]=0;
    str_score[0]=0;

    /*Score panel is cleared.*/
    for(i=0;i<curr_cave->dim_col;++i){
        al_draw_bitmap(empty_cell, g->cam_pos.c+(i*CELL_SIZE), g->cam_pos.r, 0);
    }

    if(curr_cave->dia_req>0){
        int_to_str(str_dia_req, curr_cave->dia_req);
        int_to_str(str_dia_val, curr_cave->dia_val);

        al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(2*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_dia_req);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(3*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(5*CELL_SIZE+4), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_dia_val);

    }else if(curr_cave->dia_req<=0){
        int_to_str(str_ex_dia_val, curr_cave->ex_dia_val);

        al_draw_bitmap(small_diamond, g->cam_pos.c+(1*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(2*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_bitmap(small_diamond, g->cam_pos.c+(3*CELL_SIZE), g->cam_pos.r , 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(5*CELL_SIZE+4), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_ex_dia_val);
    }
    int_to_str(str_collected_dia, g->miner.collected_dia);
    int_to_str(str_max_time, curr_cave->max_time);
    int_to_str(str_score, g->miner.score);
    al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(9*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_collected_dia);
    al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(12*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_max_time);
    al_draw_text(font, al_map_rgb(255, 255, 255), g->cam_pos.c+(15*CELL_SIZE), g->cam_pos.r, ALLEGRO_ALIGN_CENTRE, str_score);
    return;
}

void
restart_cave(Game *g,Cave *curr_cave){

    Cave *temp_cave;

    for(    temp_cave=g->head_cave;
            string_cmp(temp_cave->cave_name, curr_cave->cave_name)!=0;
            temp_cave=temp_cave->next
        );

    copy_cave(curr_cave, temp_cave);
    find_miner_loc(curr_cave, &(g->miner));
    g->miner.score=(g->miner.score)-(g->miner.curr_cave_score);
    g->miner.curr_cave_score=0;
    g->miner.collected_dia=0;
    return;
}
