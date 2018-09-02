#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"cave.h"
#include"overhead.h"
#include"insect.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"


void
find_insects(Cave *cave){
    int r,c;
    Monster *curr_monster, *pre_monster;
    Spider *curr_spider, *pre_spider;

    curr_monster=NULL;
    pre_monster=NULL;
    curr_spider=NULL;
    pre_spider=NULL;

    for(c=0; c<(cave->dim_col); ++c){
        for(r=0; r<(cave->dim_row); ++r){
            if(cave->content[r][c]==SPIDER){
                // fprintf(stderr, "spider pos [%d %d]\n", r,c);
                pre_spider=curr_spider;
                curr_spider=(Spider*)malloc(sizeof(Spider));
                curr_spider->pos.r=r;
                curr_spider->pos.c=c;
                curr_spider->move_dir=NONE;
                curr_spider->next=pre_spider;
            }else if(cave->content[r][c]==MONSTER){
                // fprintf(stderr, "monster pos [%d %d]\n", r,c);
                pre_monster=curr_monster;
                curr_monster=(Monster*)malloc(sizeof(Monster));
                curr_monster->pos.r=r;
                curr_monster->pos.c=c;
                curr_monster->route=NULL;
                curr_monster->route_size=0;
                curr_monster->next=pre_monster;
            }
        }
    }
    cave->head_monster=curr_monster;
    cave->head_spider=curr_spider;

    return;
}

void
move_insects(Game *g, Cave *curr_cave){
    Spider *curr_spider;

    for(curr_spider=curr_cave->head_spider;
        curr_spider!=NULL;
        curr_spider=curr_spider->next){
            move_spider(curr_cave, curr_spider);
        }

    return;
}

/*Spiders follow right, bottom, left, top squence if the target cell is empty.*/
void
move_spider(Cave *curr_cave, Spider *curr_spider){

    calc_spiders_route(curr_cave, curr_spider, NONE);

    if(curr_spider->move_dir!=NONE){
        curr_cave->content[curr_spider->pos.r][curr_spider->pos.c]=EMPTY_CELL;
        curr_cave->content[curr_spider->next_pos.r][curr_spider->next_pos.c]=SPIDER;
        curr_spider->pos=curr_spider->next_pos;
        //fprintf(stderr, "pos:%d,%d dir:%d\n", curr_spider->pos.r, curr_spider->pos.c, curr_spider->move_dir);
    }

    return;
}

Direction
can_spider_move(Cave *cave, Point pos, Direction dir){

    /*Spiders movement loop: right, bottom, left, top.(if the cells are empty)*/
    switch(dir){
        case NONE:
            if(cave->content[pos.r][pos.c+1]==EMPTY_CELL)/*RIGHT*/
                dir=RIGHT;
            else if(cave->content[pos.r+1][pos.c]==EMPTY_CELL)/*DOWN*/
                dir=DOWN;
            else if(cave->content[pos.r][pos.c-1]==EMPTY_CELL)/*LEFT*/
                dir=LEFT;
            else if(cave->content[pos.r-1][pos.c]==EMPTY_CELL)/*UP*/
                dir=UP;
            else
                dir=NONE;
            break;
        case RIGHT:
            if(cave->content[pos.r][pos.c+1]==EMPTY_CELL)/*RIGHT*/
                dir=RIGHT;
            else if(cave->content[pos.r+1][pos.c]==EMPTY_CELL)/*DOWN*/
                dir=DOWN;
            else if(cave->content[pos.r][pos.c-1]==EMPTY_CELL)/*LEFT*/
                dir=LEFT;
            else if(cave->content[pos.r-1][pos.c]==EMPTY_CELL)/*UP*/
                dir=UP;
            else
                dir=NONE;
            break;
        case DOWN:
            if(cave->content[pos.r+1][pos.c]==EMPTY_CELL)/*DOWN*/
                dir=DOWN;
            else if(cave->content[pos.r][pos.c-1]==EMPTY_CELL)/*LEFT*/
                dir=LEFT;
            else if(cave->content[pos.r-1][pos.c]==EMPTY_CELL)/*UP*/
                dir=UP;
            else if(cave->content[pos.r][pos.c+1]==EMPTY_CELL)/*RIGHT*/
                dir=RIGHT;
            else
                dir=NONE;
            break;
        case LEFT:
            if(cave->content[pos.r][pos.c-1]==EMPTY_CELL)/*LEFT*/
                dir=LEFT;
            else if(cave->content[pos.r-1][pos.c]==EMPTY_CELL)/*UP*/
                dir=UP;
            else if(cave->content[pos.r][pos.c+1]==EMPTY_CELL)/*RIGHT*/
                dir=RIGHT;
            else if(cave->content[pos.r+1][pos.c]==EMPTY_CELL)/*DOWN*/
                dir=DOWN;
            else
                dir=NONE;
            break;
        case UP:
            if(cave->content[pos.r][pos.c-1]==EMPTY_CELL)/*LEFT*/
                dir=LEFT;
            else if(cave->content[pos.r-1][pos.c]==EMPTY_CELL)/*UP*/
                dir=UP;
            else if(cave->content[pos.r][pos.c+1]==EMPTY_CELL)/*RIGHT*/
                dir=RIGHT;
            else if(cave->content[pos.r+1][pos.c]==EMPTY_CELL)/*DOWN*/
                dir=DOWN;
            else
                dir=NONE;
            break;
    }

    return dir;
}

void
calc_spiders_route(Cave *curr_cave, Spider *s, Direction dir){

    s->move_dir=can_spider_move(curr_cave, s->pos, s->move_dir);

    switch(s->move_dir){
        case RIGHT:
            s->next_pos.r=s->pos.r;
            s->next_pos.c=s->pos.c+1;
            break;
        case DOWN:
            s->next_pos.r=s->pos.r+1;
            s->next_pos.c=s->pos.c;
            break;
        case LEFT:
            s->next_pos.r=s->pos.r;
            s->next_pos.c=s->pos.c-1;
            break;
        case UP:
            s->next_pos.r=s->pos.r-1;
            s->next_pos.c=s->pos.c;
            break;
        case NONE:
            s->next_pos=s->pos;
            break;
    }
    return;
}

bool
is_miner_caught(Point miner_pos, Point insect_pos){
    bool caught;

    fprintf(stderr, "miner pos (%d %d)\n",miner_pos.r, miner_pos.c);
    fprintf(stderr, "Controlled pos is {%d %d}\n",insect_pos.r, insect_pos.c);
    caught=false;
    if( (miner_pos.r == (insect_pos.r-1) && miner_pos.c==insect_pos.c) ||
        (miner_pos.r == (insect_pos.r+1) && miner_pos.c==insect_pos.c) ||
        (miner_pos.c == (insect_pos.c-1) && miner_pos.r==insect_pos.r) ||
        (miner_pos.c == (insect_pos.c+1) && miner_pos.r==insect_pos.r)
    ){
        caught=true;
        // fprintf(stderr, "I have found miner.\n");
    }

    return caught;
}

bool
monster_move_possible(Cave *cave, Point *curr_route, int curr_route_size, Point pos, Direction dir){
    bool move_possible;
    // fprintf(stderr, "[monster move possible] cave dim (%d %d)\n",cave->dim_row, cave->dim_col);
    // fprintf(stderr, "[monster move possible] dir :%d, pos:{%d %d} \n",dir, pos.r, pos.c);
    move_possible=false;
    switch(dir){
        case DOWN:
            ++pos.r;
            break;
        case RIGHT:
            ++pos.c;
            break;
        case UP:
            --pos.r;
            break;
        case LEFT:
            --pos.c;
            break;
        case NONE:
            break;
    }

    if( dir!=NONE &&
        pos.r>=0 && pos.r<(cave->dim_row) && pos.c>=0 && pos.c<(cave->dim_col) &&
        (!is_pos_in_route(curr_route, curr_route_size, pos)) &&
        cave->content[pos.r][pos.c]==EMPTY_CELL){

        // fprintf(stderr, "permitted pos:{%d %d}\n",pos.r, pos.c);
        move_possible=true;
    }
    else{
        // fprintf(stderr ,"[%d %d] is not permitted.\n", pos.r, pos.c);
    }
    // fprintf(stderr , "end of monster move possible function permission %d\n",move_possible);

    return move_possible;
}
