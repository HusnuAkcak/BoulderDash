#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"cave.h"
#include"overhead.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
control_falling(Miner *m, Cave *cave) {
    /*Since display cell required a Point parameter and for clarification,
    Point types are used. */
    Point target;
    Point pos;
    bool falling;
    int r,c;

    falling=false;
    /*border is not controlled.                                             */
    for(r=(cave->dim_row)-1; r>0; --r){
        for(c=(cave->dim_col)-1; c>0; --c){
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

                /*A falling rock can die miner, spider or a monster.        */
                if(cave->content[target.r][target.c]==ROCK){
                   if(cave->content[target.r+1][target.c]==MINER){
                       cave->content[target.r+1][target.c]=DEAD_MINER;
                       m->alive=false;
                    }
                    control_crushed_insects(cave, target);
                }
                /*related bitmaps are being updated.                        */
                display_cell(pos , cave);
                display_cell(target, cave);
                falling=false;
            }
        }
    }
    return;
}

/*This function is called after any rock is falled.                         */
void
control_crushed_insects(Cave *cave, Point rock_pos){
    Point spider_dia_arr [DIA_OF_SPIDER];
    Point monster_dia_arr [DIA_OF_MONSTER];
    Spider *temp_spider, *curr_spider;
    Monster *temp_monster, *curr_monster;
    bool monster_crush, spider_crush;/*Is current insect crushed.           */

    spider_crush=false;
    /*Spiders are controlled, if one of them is under the rock              */
    curr_spider=cave->head_spider;
    if(curr_spider!=NULL){
        /*if crushed spider is head_spider(head node)                       */
        if((curr_spider->pos.r)==rock_pos.r+1 && (curr_spider->pos.c)==rock_pos.c){
            spider_crush=true;
            temp_spider=curr_spider;
            curr_spider=curr_spider->next;
            cave->head_spider=curr_spider;/*head is redetermined.           */
            free(temp_spider);
            temp_spider=NULL;
        }

        /*connector nodes are controlled.                                   */
        while(!spider_crush && curr_spider!=NULL && curr_spider->next!=NULL){
            if((curr_spider->next->pos.r)==rock_pos.r+1 && (curr_spider->next->pos.c)==rock_pos.c){
                spider_crush=true;
                temp_spider=curr_spider->next;
                curr_spider->next=curr_spider->next->next;
                free(temp_spider);
                temp_spider=NULL;
            }
            curr_spider=curr_spider->next;
        }
    }

    /*Monsters are controlled, if one of them is under the rock              */
    curr_monster=cave->head_monster;
    if(!spider_crush && curr_monster!=NULL){
        /*if crushed monster is head_monster(head node)                      */
        if((curr_monster->pos.r)==rock_pos.r+1 && (curr_monster->pos.c)==rock_pos.c){
            monster_crush=true;
            temp_monster=curr_monster;
            curr_monster=curr_monster->next;
            cave->head_monster=curr_monster;/*head is redetermined.         */
            free(temp_monster);
            temp_monster=NULL;
        }

        /*connector nodes are controlled.                                   */
        while(!monster_crush && curr_monster!=NULL && curr_monster->next!=NULL){
            if((curr_monster->next->pos.r)==rock_pos.r+1 && (curr_monster->next->pos.c)==rock_pos.c){
                monster_crush=true;
                temp_monster=curr_monster->next;
                curr_monster->next=curr_monster->next->next;
                free(temp_monster);
                temp_monster=NULL;
            }
            curr_monster=curr_monster->next;
        }
    }

    /*the rock and the insect are deleted from the map.                     */
    if(spider_crush || monster_crush){

        cave->content[rock_pos.r][rock_pos.c]=EMPTY_CELL;
        cave->content[rock_pos.r+1][rock_pos.c]=EMPTY_CELL;

        rock_pos.r+=1;/*it is used for insect position.                     */
        if(spider_crush){
            find_available_cells_for_dia(cave, spider_dia_arr, DIA_OF_SPIDER, rock_pos);
            fill_available_cells_with_dia(cave, spider_dia_arr, DIA_OF_SPIDER);
        }
        else if(monster_crush){
            find_available_cells_for_dia(cave, monster_dia_arr, DIA_OF_MONSTER, rock_pos);
            fill_available_cells_with_dia(cave, monster_dia_arr, DIA_OF_MONSTER);
        }

    }

    return;
}

void
find_available_cells_for_dia(Cave *cave, Point tar_cells[], int arr_size, Point ins_pos){
    int radius;     /*current radius of the dead insect */
    int determined; /*determined target number          */
    Point curr_cell;

    radius=0;
    tar_cells[0]=ins_pos;/*Dead insect's pos is always will be available.   */
    determined=1;/*one target is already determined above.                  */
    while((arr_size-determined)>0){
        ++radius;
        /*left side control*/
        if((ins_pos.c-radius)>0 &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r][ins_pos.c-radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r][ins_pos.c-radius]==SOIL)   ){
            curr_cell.r=ins_pos.r;
            curr_cell.c=ins_pos.c-radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*right side control*/
        if((ins_pos.c+radius)<(cave->dim_col) &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r][ins_pos.c+radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r][ins_pos.c+radius]==SOIL)   ){
            curr_cell.r=ins_pos.r;
            curr_cell.c=ins_pos.c+radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*underside control*/
        if((ins_pos.r+radius)<(cave->dim_row) &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r+radius][ins_pos.c]==EMPTY_CELL ||
                        cave->content[ins_pos.r+radius][ins_pos.c]==SOIL)   ){
            curr_cell.r=ins_pos.r+radius;
            curr_cell.c=ins_pos.c;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*upside control*/
        if((ins_pos.r-radius)>0 &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r-radius][ins_pos.c]==EMPTY_CELL ||
                        cave->content[ins_pos.r-radius][ins_pos.c]==SOIL)   ){
            curr_cell.r=ins_pos.r-radius;
            curr_cell.c=ins_pos.c;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*right underside diagonal control*/
        if((ins_pos.r+radius)<(cave->dim_row) && (ins_pos.c+radius)<(cave->dim_col) &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r+radius][ins_pos.c+radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r+radius][ins_pos.c+radius]==SOIL)   ){
            curr_cell.r=ins_pos.r+radius;
            curr_cell.c=ins_pos.c+radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*right upside diagonal control*/
        if((ins_pos.c+radius)<(cave->dim_col) && (ins_pos.r-radius)>0 &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r-radius][ins_pos.c+radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r-radius][ins_pos.c+radius]==SOIL)   ){
            curr_cell.r=ins_pos.r-radius;
            curr_cell.c=ins_pos.c+radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*left upside diagonal control*/
        if((ins_pos.c-radius)>0 && (ins_pos.r-radius)>0 &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r-radius][ins_pos.c-radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r-radius][ins_pos.c-radius]==SOIL)   ){
            curr_cell.r=ins_pos.r-radius;
            curr_cell.c=ins_pos.c-radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
        /*left underside diagonal control*/
        if((ins_pos.c-radius)>0 && (ins_pos.r+radius)<(cave->dim_row) &&
            (arr_size-determined)>0 &&
            (cave->content[ins_pos.r+radius][ins_pos.c-radius]==EMPTY_CELL ||
                        cave->content[ins_pos.r+radius][ins_pos.c-radius]==SOIL)   ){
            curr_cell.r=ins_pos.r+radius;
            curr_cell.c=ins_pos.c-radius;
            tar_cells[determined]=curr_cell;
            ++determined;
        }
    }

    return;
}

void
fill_available_cells_with_dia(Cave *curr_cave, Point tar_cells[], int arr_size){

    int i;
    Point curr_cell;

    for(i=0; i<arr_size; ++i){
        curr_cell=tar_cells[i];
        curr_cave->content[curr_cell.r][curr_cell.c]=DIAMOND;
    }

    return;
}

void
detect_target(Cave *cave, Miner *m, char *target, char *after_target, Point *tp, Point *atp) {

    /*We can reach target and after target from tp and atp but for convinience,
    we keep the two kind of data.*/

    /*target and after target are determined according to direction.        */
    if(m->move_dir==UP){
        tp->c=m->pos.c;
        tp->r=m->pos.r-1;
        atp->c=tp->c;
        if(((tp->r)-1)>=0){ /*if it is in the region                        */
            atp->r=(tp->r)-1;
        }else{              /*else atp is come up to tp                     */
            atp->r=tp->r;
        }
    }else if(m->move_dir==RIGHT){
        tp->r=m->pos.r;
        tp->c=m->pos.c+1;
        atp->r=tp->r;
        if( ((tp->c)+1)<(cave->dim_col) ){
            atp->c=(tp->c)+1;
        }else{
            atp->c=tp->c;
        }
    }else if(m->move_dir==DOWN){
        tp->c=m->pos.c;
        tp->r=m->pos.r+1;
        atp->c=tp->c;
        if( ((tp->r)+1)<(cave->dim_row) ){
            atp->r=(tp->r)+1;
        }else{
            atp->r=tp->r;
        }
    }else if(m->move_dir==LEFT){
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

void
is_miner_dead(Game *g, Cave *curr_cave, Miner *m) {

    if( curr_cave->content[m->pos.r-1][m->pos.c] ==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c] ==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c-1] ==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c+1] ==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c] ==SPIDER
        || curr_cave->content[m->pos.r-1][m->pos.c] ==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c-1] ==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c+1] ==SPIDER
        || (curr_cave->left_time <=0 && g->status!=NEXT)
    ){
        curr_cave->content[m->pos.r][m->pos.c]=DEAD_MINER;
        m->alive=false;
    }
    return;
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
