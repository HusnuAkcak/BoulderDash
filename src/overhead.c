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
    /*Since display cell required a Point parameter and for clarification,
    Point types are used. */
    Point target;
    Point pos;
    bool falling;
    int r,c;

    falling=false;
    /*border is not controlled.                                             */
    for(r=(cave->dim_row)-1;r>0;--r){
        for(c=(cave->dim_col)-1;c>0;--c){
            /*underside control         */
            if( (cave->content[r][c]==DIAMOND || cave->content[r][c]==ROCK) &&
                                            cave->content[r+1][c]==EMPTY_CELL ){
                falling=true;
                pos.r=r;
                pos.c=c;
                target.r=r+1;
                target.c=c;
            }
            /*left diagonal control    */
            else if(
                     (cave->content[r][c]==DIAMOND || cave->content[r][c]==ROCK) &&
                     (cave->content[r+1][c]==DIAMOND || cave->content[r+1][c]==ROCK) &&
                     cave->content[r][c-1]==EMPTY_CELL && cave->content[r+1][c-1]==EMPTY_CELL ){
                falling=true;
                pos.r=r;
                pos.c=c;
                target.r=r+1;
                target.c=c-1;
            }
            /*right diagonal control   */
            else if(
                     (cave->content[r][c]==DIAMOND || cave->content[r][c]==ROCK) &&
                     (cave->content[r+1][c]==DIAMOND || cave->content[r+1][c]==ROCK) &&
                     cave->content[r][c+1]==EMPTY_CELL && cave->content[r+1][c+1]==EMPTY_CELL ){
                falling=true;
                pos.r=r;
                pos.c=c;
                target.r=r+1;
                target.c=c+1;
            }

            if(falling){
                cave->content[target.r][target.c]=cave->content[pos.r][pos.c];
                cave->content[pos.r][pos.c]=EMPTY_CELL;
                /*related bitmaps are being updated.                        */
                display_cell(pos , cave);
                display_cell(target, cave);
                falling=false;
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

bool
is_miner_dead(Cave *curr_cave, Miner *m){
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

void
set_camera(Game *g, Cave *curr_cave){
    double screen_height, screen_width, cave_height, cave_width;
    Point camera_pos;

    screen_height=al_get_display_height(display);
    screen_width=al_get_display_width(display);
    cave_height=(curr_cave->dim_row+1)*CELL_SIZE;/* '+1' from score panel   */
    cave_width=(curr_cave->dim_col)*CELL_SIZE;

    /*Rest of the function is quite standart process for camera operations  */

    /*It is desired to keep miner center of the screen                      */
    camera_pos.r=(g->miner.pos.r*CELL_SIZE)-(screen_height/2);
    camera_pos.c=(g->miner.pos.c*CELL_SIZE)-(screen_width/2);

    /*if camera is not in cave border it is adjusted                        */
    if(camera_pos.r<0)
        camera_pos.r=0;
    else if(camera_pos.r>(cave_height-screen_height))
        camera_pos.r=cave_height-screen_height+CELL_SIZE;

    /*if camera is not in cave border it is adjusted                        */
    if(camera_pos.c<0)
        camera_pos.c=0;
    else if(camera_pos.c>(cave_width-screen_width))
        camera_pos.c=cave_width-screen_width+CELL_SIZE;

    g->cam_pos=camera_pos;
    return;
}
