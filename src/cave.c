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
display_curr_cave(Cave * cave){

    Point pos;

    if(!al_init_image_addon()){
        al_show_native_message_box(display,"Error","Error",
        "Failed to initialize al_init_image_addon",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }
    al_clear_to_color(al_map_rgb(128, 153, 51));
    for(pos.r=0;(pos.r)<(cave->dim_row);++pos.r){
        for(pos.c=0;(pos.c)<(cave->dim_col);++pos.c){
            display_cell(pos, cave);
        }
    }
    return;
}

void
display_cell(Point pos,Cave* cave){
    Content content;

    content=cave->content[pos.r][pos.c];
    /*Firstly, target cell is cleared.                                      */
    al_draw_bitmap(empty_cell, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);

    switch(content){
        case EX_WALL:
            al_draw_bitmap(ex_wall, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case IN_WALL:
            al_draw_bitmap(in_wall, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case SOIL:
            al_draw_bitmap(soil, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case DIAMOND:
            al_draw_bitmap(diamond, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case ROCK:
            al_draw_bitmap(rock, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case MINER:
            al_draw_bitmap(miner, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case MONSTER:
            al_draw_bitmap(monster, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case GATE:
            al_draw_bitmap(gate, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case SPIDER:
            al_draw_bitmap(spider, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case WATER:
            al_draw_bitmap(water, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        case EMPTY_CELL:
            al_draw_bitmap(empty_cell, (pos.c)*CELL_SIZE, CELL_SIZE+((pos.r)*CELL_SIZE), 0);
            break;
        }
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
    display_curr_cave(curr_cave);
    display_score_panel(curr_cave, &(g->miner));
    al_flip_display();
    return;
}
