#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"cave.h"
#include"overhead.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
control_falling(Cave *cave){
    Point underside;
    Point pos;
    int r,c;

    /*border is not controlled.                                             */
    for(r=1;r<(cave->dim_row)-1;++r){
        for(c=1;c<(cave->dim_col)-1;++c){

            if( (cave->content[r][c]==DIAMOND || cave->content[r][c]==ROCK) &&
                                            cave->content[r+1][c]==EMPTY_CELL ){
                cave->content[r+1][c]=cave->content[r][c];
                cave->content[r][c]=EMPTY_CELL;

                /*related bitmaps are being updated.                        */
                pos.r=r;
                pos.c=c;
                underside.r=r+1;
                underside.c=c;
                display_cell(pos , cave);
                display_cell(underside, cave);
            }

        }
    }
    return;
}

void
detect_target(Direction dir, Cave *cave, Miner *m,
    char *target, char *after_target, Point *tp,Point *atp){

    /*We can reach target and after target from tp and atp but for convinience,
    we keep the two kind of data.*/

    /*target and after target are determined according to direction.        */
    if(dir==UP){
        tp->c=m->pos.c;
        tp->r=m->pos.r-1;
        atp->c=tp->c;
        if(((tp->r)-1)>=0){ /*if it is in the region                        */
            atp->r=(tp->r)-1;
        }else{              /*else atp is come up to tp                     */
            atp->r=tp->r;
        }
    }else if(dir==RIGHT){
        tp->r=m->pos.r;
        tp->c=m->pos.c+1;
        atp->r=tp->r;
        if( ((tp->c)+1)<(cave->dim_col) ){
            atp->c=(tp->c)+1;
        }else{
            atp->c=tp->c;
        }
    }else if(dir==DOWN){
        tp->c=m->pos.c;
        tp->r=m->pos.r+1;
        atp->c=tp->c;
        if( ((tp->r)+1)<(cave->dim_row) ){
            atp->r=(tp->r)+1;
        }else{
            atp->r=tp->r;
        }
    }else if(dir==LEFT){
        tp->r=m->pos.r;
        tp->c=m->pos.c-1;
        atp->r=tp->r;
        if(((tp->c)-1)>=0){
            atp->c=(tp->c)-1;
        }else{
            atp->c=tp->c;
        }
    }

    *target=cave->content[tp->r][tp->c];
    *after_target=cave->content[atp->r][atp->c];

    return;
}

bool is_miner_dead(Cave *curr_cave, Miner *m){
    bool dead;

    dead=false;
    if( curr_cave->content[m->pos.r-1][m->pos.c] ==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c] ==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c-1] ==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c+1] ==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c] ==SPIDER
        || curr_cave->content[m->pos.r-1][m->pos.c] ==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c-1] ==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c+1] ==SPIDER
        || curr_cave->max_time <=0
    ){
        dead=true;
    }
    return dead;
}



void
find_miner_loc(Cave *curr_cave, Miner *m){

    int r,c;

    for(r=0;r<curr_cave->dim_row;++r){
        for(c=0;c<curr_cave->dim_col;++c){
            if(curr_cave->content[r][c]==MINER){
                m->pos.r=r;
                m->pos.c=c;
            }
        }
    }
    return;
}
