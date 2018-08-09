#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"cave.h"
#include"overhead.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
control_falling(Cave *cave, Point pos){
    Point upside;

    upside=pos;
    upside.r=pos.r-1;

    if( (cave->content[upside.r][upside.c]==DIAMOND || cave->content[upside.r][upside.c]==ROCK) &&
                                                cave->content[pos.r][pos.c]==EMPTY_CELL ){
        cave->content[pos.r][pos.c]=cave->content[upside.r][upside.c];
        cave->content[upside.r][upside.c]=EMPTY_CELL;
        /*related bitmaps are being updated.                                */
        display_cell(pos , cave);
        display_cell(upside, cave);
    }
    /*if there is somethings upperside                                      */
    if((upside.r-1)>=0 && (cave->content[upside.r-1][upside.c]==ROCK || cave->content[upside.r-1][upside.c]== DIAMOND)){
        upside.r-=1;
        control_falling(cave, upside);
    }
    /*if the object is still falling.                                       */
    if((pos.r+1)<(cave->dim_row) && cave->content[pos.r+1][pos.c]==EMPTY_CELL){
        pos.r+=1;
        control_falling(cave, pos);
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
    if( curr_cave->content[m->pos.r-1][m->pos.c]==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c]==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c-1]==MONSTER
        || curr_cave->content[m->pos.r][m->pos.c+1]==MONSTER
        || curr_cave->content[m->pos.r+1][m->pos.c]==SPIDER
        || curr_cave->content[m->pos.r-1][m->pos.c]==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c-1]==SPIDER
        || curr_cave->content[m->pos.r][m->pos.c+1]==SPIDER
    ){
        dead=true;
    }
    return dead;
}

void
display_score_panel(Cave *curr_cave, Miner *m){
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
        al_draw_bitmap(empty_cell, i*CELL_SIZE, 0, 0);
    }

    // fprintf(stderr, "%d, %d ,%d ,%d ,%d ,%d\n",curr_cave->dia_req, curr_cave->dia_val,
    //     curr_cave->ex_dia_val, m->collected_dia, curr_cave->max_time,m->score);

    if(curr_cave->dia_req>0){
        int_to_str(str_dia_req, curr_cave->dia_req);
        int_to_str(str_dia_val, curr_cave->dia_val);

        // al_draw_text(font, al_map_rgb(255, 255, 255), CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, "dia_req:");
        al_draw_text(font, al_map_rgb(255, 255, 255), 2*CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, str_dia_req);
        al_draw_bitmap(small_diamond, 3*CELL_SIZE, 8 , 0);
        // al_draw_text(font, al_map_rgb(255, 255, 255), 6*CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, "dia_val:");
        al_draw_text(font, al_map_rgb(255, 255, 255), 4*CELL_SIZE+4, 0,ALLEGRO_ALIGN_CENTRE, str_dia_val);

    }else if(curr_cave->dia_req<=0){
        int_to_str(str_ex_dia_val, curr_cave->ex_dia_val);

        al_draw_bitmap(small_diamond, 1*CELL_SIZE, 8 , 0);
        al_draw_bitmap(small_diamond, 2*CELL_SIZE, 8 , 0);
        al_draw_bitmap(small_diamond, 3*CELL_SIZE, 8 , 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 4*CELL_SIZE+4, 0,ALLEGRO_ALIGN_CENTRE, str_ex_dia_val);
    }
    int_to_str(str_collected_dia, m->collected_dia);
    int_to_str(str_max_time, curr_cave->max_time);
    int_to_str(str_score, m->score);
    al_draw_text(font, al_map_rgb(255, 255, 255), 9*CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, str_collected_dia);
    al_draw_text(font, al_map_rgb(255, 255, 255), 12*CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, str_max_time);
    al_draw_text(font, al_map_rgb(255, 255, 255), 15*CELL_SIZE, 0,ALLEGRO_ALIGN_CENTRE, str_score);
    return;
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
