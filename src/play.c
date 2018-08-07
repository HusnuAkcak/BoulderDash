#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
start_game(Game *game,int scr_width,int scr_height){

    ALLEGRO_BITMAP      *image1;
    ALLEGRO_BITMAP      *image2;
    ALLEGRO_SAMPLE      *sample;

    /*intro music                                                      */
    al_reserve_samples(1);
    sample=al_load_sample(AUDIO_PATH"/Music.wav");
    al_play_sample(sample,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

    image1=al_load_bitmap(IMG_PATH"/boulder_dash.jpg");
    al_draw_bitmap(image1,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.5);
    al_clear_to_color(al_map_rgb(0,0,0));
    image2=al_load_bitmap(IMG_PATH"/title_screen.png");
    al_draw_bitmap(image2,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.5);
    al_clear_to_color(al_map_rgb(0,0,0));

    game->miner.life=MINER_LIFE;
    game->miner.score=0;
    game->miner.curr_cave_score=0;
    display_game(game);

    al_destroy_bitmap(image1);
    al_destroy_bitmap(image2);
    al_destroy_sample(sample);
}

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
    display_curr_cave(curr_cave);
    return;
}

void
display_game(Game * g){

    Cave curr_cave;
    ALLEGRO_EVENT ev;
    Status status;  /*status of the game(continue ,restart, end)            */

    status=CONTINUE;
    curr_cave.content=NULL;
    curr_cave.dim_row=0;
    curr_cave.dim_col=0;
    copy_cave(&curr_cave, g->head_cave);
    find_miner_loc(&curr_cave, &(g->miner));    /*Miner's location is found. */
    display_curr_cave(&curr_cave);
    while(status!=END){
        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            status=END;
        }else if(status==RESTART){
            restart_cave(g, &curr_cave);
            status=CONTINUE;
        }else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE){
                status=END;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN){
                status=move(&curr_cave,&(g->miner),DOWN);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
                status=move(&curr_cave,&(g->miner),UP);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT){
                status=move(&curr_cave,&(g->miner),LEFT);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT){
                status=move(&curr_cave,&(g->miner),RIGHT);
            }
        }
    }
    return;
}

void
display_curr_cave(Cave * cave){

    int row,col;

    if(!al_init_image_addon()){
        al_show_native_message_box(display,"Error","Error",
        "Failed to initialize al_init_image_addon",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }
    al_clear_to_color(al_map_rgb(0,0,0));
    for(row=0;row<cave->dim_row;++row){
        for(col=0;col<cave->dim_col;++col){
            display_cell(row, col, cave->content[row][col]);
        }
    }
    al_flip_display();
    return;
}

void
display_cell(int row, int col,Content content){

    switch(content){
        case EX_WALL:
            al_draw_bitmap(ex_wall, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case IN_WALL:
            al_draw_bitmap(in_wall, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case SOIL:
            al_draw_bitmap(soil, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case DIAMOND:
            al_draw_bitmap(diamond, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case ROCK:
            al_draw_bitmap(rock, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case MINER:
            al_draw_bitmap(miner, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case MONSTER:
            al_draw_bitmap(monster, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case GATE:
            al_draw_bitmap(gate, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case SPIDER:
            al_draw_bitmap(spider, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case WATER:
            al_draw_bitmap(water, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        case EMPTY_CELL:
            al_draw_bitmap(empty_cell, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            break;
        }
}

Status
move(Cave * curr_cave,Miner *m,Direction dir){

    char target;    /*target cell                                           */
    int r,c;        /*row and column                                        */
    Status status;  /*status of the game(restart, continue, end)            */

    status=CONTINUE;

    /*target is determined                                                  */
    if(dir==UP){
        target=curr_cave->content[m->coord_r-1][m->coord_c];
    }else if(dir==RIGHT){
        target=curr_cave->content[m->coord_r][m->coord_c+1];
    }else if(dir==DOWN){
        target=curr_cave->content[m->coord_r+1][m->coord_c];
    }else if(dir==LEFT){
        target=curr_cave->content[m->coord_r][m->coord_c-1];
    }

    /*it is controlled and if the move possible player's choice is applied. */
    if(target!=IN_WALL && target !=EX_WALL && target!=ROCK && target!=WATER){
        curr_cave->content[m->coord_r][m->coord_c]=EMPTY_CELL;
        display_cell(m->coord_r, m->coord_c,EMPTY_CELL);

        if(dir==UP){
            m->coord_r=m->coord_r-1;
        }else if(dir==RIGHT){
            m->coord_c=m->coord_c+1;
        }else if(dir==DOWN){
            m->coord_r=m->coord_r+1;
        }else if(dir==LEFT){
            m->coord_c=m->coord_c-1;
        }

        if(is_miner_dead(curr_cave, m)==true){

            --m->life;
            display_cell(m->coord_r, m->coord_c, MINER);

            if(m->life>0)
                status=RESTART;
            else
                status=END;
        }else if(curr_cave->content[m->coord_r][m->coord_c]==GATE){
            // curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c, MINER);
            if(curr_cave->dia_req<=0 && curr_cave->next!=NULL){
                curr_cave=curr_cave->next;
                /*For new level, the miner's location is found.             */
                for(r=0;r<curr_cave->dim_row;++r){
                    for(c=0;c<curr_cave->dim_col;++c){
                        if(curr_cave->content[r][c]==MINER){
                            m->coord_r=r;
                            m->coord_c=c;
                        }
                    }
                }
            }
            else if(curr_cave->dia_req<=0 && curr_cave->next==NULL){
                status=END;
            }
        }
        else if(curr_cave->content[m->coord_r][m->coord_c]==SOIL ||
                    curr_cave->content[m->coord_r][m->coord_c]==' '){
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c, EMPTY_CELL);
            display_cell(m->coord_r, m->coord_c, MINER);
        }else if(curr_cave->content[m->coord_r][m->coord_c]==DIAMOND){
            if(curr_cave->dia_req>0){
                --curr_cave->dia_req;
                m->curr_cave_score+=curr_cave->dia_val;
                m->score+=curr_cave->dia_val;
            }else{
                m->curr_cave_score+=curr_cave->ex_dia_val;
                m->score+=curr_cave->ex_dia_val;
            }
            /*Board view is adjusted.                                       */
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c,EMPTY_CELL);
            display_cell(m->coord_r, m->coord_c, MINER);
        }

    }
    al_flip_display();
    return status;
}

bool is_miner_dead(Cave *curr_cave, Miner *m){
    bool dead;

    dead=false;
    if( curr_cave->content[m->coord_r-1][m->coord_c]==MONSTER
        || curr_cave->content[m->coord_r+1][m->coord_c]==MONSTER
        || curr_cave->content[m->coord_r][m->coord_c-1]==MONSTER
        || curr_cave->content[m->coord_r][m->coord_c+1]==MONSTER
        || curr_cave->content[m->coord_r+1][m->coord_c]==SPIDER
        || curr_cave->content[m->coord_r-1][m->coord_c]==SPIDER
        || curr_cave->content[m->coord_r][m->coord_c-1]==SPIDER
        || curr_cave->content[m->coord_r][m->coord_c+1]==SPIDER
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
                m->coord_r=r;
                m->coord_c=c;
            }
        }
    }
    return;
}
