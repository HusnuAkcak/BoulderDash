#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"cave.h"
#include"overhead.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
control_falling(Cave *cave) {
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
detect_target(Direction dir, Cave *cave, Miner *m,char *target, char *after_target, Point *tp,Point *atp) {

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
is_miner_dead(Cave *curr_cave, Miner *m) {
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
find_miner_loc(Cave *curr_cave, Miner *m) {

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
set_camera(Game *g, Cave *curr_cave) {
    Point camera_pos, screen_dim, cave_dim;

    screen_dim.r=al_get_display_height(display);
    screen_dim.c=al_get_display_width(display);
    cave_dim.r=(curr_cave->dim_row)*CELL_SIZE;
    cave_dim.c=(curr_cave->dim_col)*CELL_SIZE;

    /*Miner is kept center of the screen                                    */
    camera_pos.r=(g->miner.pos.r*CELL_SIZE)-(screen_dim.r/2);
    camera_pos.c=(g->miner.pos.c*CELL_SIZE)-(screen_dim.c/2);

    // fprintf(stderr, "cave_dim:%d,%d\n",cave_dim.r, cave_dim.c);
    // fprintf(stderr, "screen_dim:%d,%d\n",screen_dim.r/2, screen_dim.c/2);
    // fprintf(stderr, "miner's pos:%d,%d\n",g->miner.pos.r, g->miner.pos.c);
    /*if camera is not in cave border it is adjusted                        */
    if(camera_pos.r<0)
        camera_pos.r=0;
    else if(camera_pos.r>(cave_dim.r-screen_dim.r))
        camera_pos.r=cave_dim.r-screen_dim.r+CELL_SIZE;

    /*if camera is not in cave border it is adjusted                        */
    if(camera_pos.c<0)
        camera_pos.c=0;
    else if(camera_pos.c>(cave_dim.c-screen_dim.c))
        camera_pos.c=cave_dim.c-screen_dim.c+CELL_SIZE;

    g->cam_pos=camera_pos;
    return;
}

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
                pre_spider=curr_spider;
                curr_spider=(Spider*)malloc(sizeof(Spider));
                curr_spider->pos.r=r;
                curr_spider->pos.c=c;
                curr_spider->move_dir=NONE;
                curr_spider->next=pre_spider;
            }else if(cave->content[r][c]==MONSTER){
                pre_monster=curr_monster;
                curr_monster=(Monster*)malloc(sizeof(Monster));
                curr_monster->pos.r=r;
                curr_monster->pos.c=c;
                curr_monster->route_head=NULL;
                curr_monster->next=pre_monster;
            }
        }
    }
    cave->head_monster=curr_monster;
    cave->head_spider=curr_spider;

    return;
}

void
move_insects(Cave *curr_cave){
    Monster *curr_monster;
    Spider *curr_spider;

    for(curr_monster=curr_cave->head_monster;
        curr_monster!=NULL;
        curr_monster=curr_monster->next){
            move_monster(curr_cave, curr_monster);
        }

    for(curr_spider=curr_cave->head_spider;
        curr_spider!=NULL;
        curr_spider=curr_spider->next){
            move_spider(curr_cave, curr_spider);
        }

    return;
}

void
move_monster(Cave *curr_cave, Monster *curr_monster){

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

Direction can_spider_move(Cave *cave, Point pos, Direction dir){

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
