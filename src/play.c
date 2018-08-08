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
    game->miner.collected_dia=0;
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
    g->miner.collected_dia=0;
    display_curr_cave(curr_cave);
    display_score_panel(curr_cave, &(g->miner));
    al_flip_display();
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
    display_score_panel(&curr_cave, &(g->miner));
    al_flip_display();
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
    al_clear_to_color(al_map_rgb(128, 153, 51));
    for(row=0;row<cave->dim_row;++row){
        for(col=0;col<cave->dim_col;++col){
            display_cell(row, col, cave->content[row][col]);
        }
    }
    return;
}

void
display_cell(int row, int col,Content content){

    /*Firstly, target cell is cleared.                                      */
    al_draw_bitmap(empty_cell, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);

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

    char target;        /*target cell                                       */
    char after_target;  /*comes after target cell(in the same direction)    */
    Status status;      /*status of the game(restart, continue, end)        */

    status=CONTINUE;

    /*target is determined                                                  */
    if(dir==UP){
        target=curr_cave->content[m->coord_r-1][m->coord_c];
        if((m->coord_r-2)>=0)
            after_target=curr_cave->content[m->coord_r-2][m->coord_c];
        else
            after_target=EX_WALL;
    }else if(dir==RIGHT){
        target=curr_cave->content[m->coord_r][m->coord_c+1];
        if((m->coord_c+2)<(curr_cave->dim_col))
            after_target=curr_cave->content[m->coord_r][m->coord_c+2];
        else
            after_target=EX_WALL;
    }else if(dir==DOWN){
        target=curr_cave->content[m->coord_r+1][m->coord_c];
        if((m->coord_r+2)<(curr_cave->dim_row))
            after_target=curr_cave->content[m->coord_r+2][m->coord_c];
        else
            after_target=EX_WALL;
    }else if(dir==LEFT){
        target=curr_cave->content[m->coord_r][m->coord_c-1];
        if((m->coord_c-2)>=0)
            after_target=curr_cave->content[m->coord_r][m->coord_c-2];
        else
            after_target=EX_WALL;
    }
    //fprintf(stderr, "%c, ",target);
    /*it is controlled and if the move possible player's choice is applied. */
    if(target!=IN_WALL && target !=EX_WALL && target!=WATER &&
                                !(target==ROCK && after_target!=EMPTY_CELL)  ){
        /*previous location is changed as EMPTY_CELL                        */
        curr_cave->content[m->coord_r][m->coord_c]=EMPTY_CELL;
        display_cell(m->coord_r, m->coord_c ,curr_cave->content[m->coord_r][m->coord_c]);

        if(dir==UP){
            curr_cave->content[m->coord_r-1][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c,curr_cave->content[m->coord_r][m->coord_c]);
            m->coord_r=m->coord_r-1;
        }else if(dir==RIGHT){
            curr_cave->content[m->coord_r][m->coord_c+1]=MINER;
            display_cell(m->coord_r, m->coord_c,curr_cave->content[m->coord_r][m->coord_c]);
            control_falling(curr_cave, m->coord_r, m->coord_c);
            m->coord_c=m->coord_c+1;
        }else if(dir==DOWN){
            curr_cave->content[m->coord_r+1][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c,curr_cave->content[m->coord_r][m->coord_c]);
            control_falling(curr_cave, m->coord_r, m->coord_c);
            m->coord_r=m->coord_r+1;
        }else if(dir==LEFT){
            curr_cave->content[m->coord_r][m->coord_c-1]=MINER;
            display_cell(m->coord_r, m->coord_c,curr_cave->content[m->coord_r][m->coord_c]);
            control_falling(curr_cave, m->coord_r, m->coord_c);
            m->coord_c=m->coord_c-1;
        }

        //fprintf(stderr,"3:17;%c\n", curr_cave->content[3][17]);
        //al_flip_display();
        //fprintf(stderr, "(6,18):%c \n",curr_cave->content[6][8]);
        //fprintf(stderr, "%d,%d\n",m->coord_r,m->coord_c);

        //fprintf(stderr,"3:17;%c\n", curr_cave->content[3][17]);

        //fprintf(stderr, "%c \n",target);
        if(is_miner_dead(curr_cave, m)==true){
            --m->life;
            find_miner_loc(curr_cave, m);
            //fprintf(stderr, "%d,%d :%c\n", m->coord_r, m->coord_c, curr_cave->content[m->coord_r][m->coord_c]);
            display_cell(m->coord_r, m->coord_c, MINER);
            if(m->life>0)
                status=RESTART;
            else
                status=END;
        }else if(curr_cave->content[m->coord_r][m->coord_c]==GATE){/*this function is adjusted then.*/
            display_cell(m->coord_r, m->coord_c, MINER);
            if(curr_cave->dia_req<=0 && curr_cave->next!=NULL){
                curr_cave=curr_cave->next;
                find_miner_loc(curr_cave, m);
            }else if(curr_cave->dia_req<=0 && curr_cave->next==NULL){
                status=END;
            }
        }
        else if(target==SOIL || target==EMPTY_CELL){
            //fprintf(stderr, "The cell is empty\n");
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c, MINER);
        }else if(target==DIAMOND){
            //fprintf(stderr,"I collect a diamond!\n");
            ++m->collected_dia;
            if(curr_cave->dia_req>0){
                --curr_cave->dia_req;
                m->curr_cave_score+=curr_cave->dia_val;
                m->score+=curr_cave->dia_val;
            }else{
                m->curr_cave_score+=curr_cave->ex_dia_val;
                m->score+=curr_cave->ex_dia_val;
            }
            /*Board view is adjusted.                                       */
            //fprintf(stderr, "eat diamond %d,%d :%c\n",m->coord_r, m->coord_c, curr_cave->content[m->coord_r][m->coord_c]);
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c, MINER);
        }else if(target==ROCK && after_target==EMPTY_CELL){
            // fprintf(stderr, "I am carrying rocks!\n");
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c, MINER);
            switch(dir){
                case UP:
                    curr_cave->content[m->coord_r-1][m->coord_c]=ROCK;
                    display_cell(m->coord_r-1, m->coord_c, ROCK);
                    control_falling(curr_cave, m->coord_r-1, m->coord_c);
                    break;
                case DOWN:
                    curr_cave->content[m->coord_r+1][m->coord_c]=ROCK;
                    display_cell(m->coord_r+1, m->coord_c, ROCK);
                    control_falling(curr_cave, m->coord_r+1, m->coord_c);
                    break;
                case RIGHT:
                    curr_cave->content[m->coord_r][m->coord_c+1]=ROCK;
                    display_cell(m->coord_r, m->coord_c+1, ROCK);
                    control_falling(curr_cave, m->coord_r, m->coord_c+1);
                    break;
                case LEFT:
                    curr_cave->content[m->coord_r][m->coord_c-1]=ROCK;
                    display_cell(m->coord_r, m->coord_c-1, ROCK);
                    control_falling(curr_cave, m->coord_r, m->coord_c-1);
                    break;
            }
        }
    }
    display_score_panel(curr_cave, m);
    al_flip_display();
    /*if miner is died, screen is freezed for a while.                      */
    if(status!=CONTINUE)
        al_rest(0.5);

    return status;
}

void
control_falling(Cave *cave, int r, int c){
    //int i,j;
    //fprintf(stderr, "Falling objects are being contrelled.\n");
    //fprintf(stderr, "coord:(%d,%d) %c \n", r, c, cave->content[r][c]);
    if((cave->content[r-1][c]==DIAMOND || cave->content[r-1][c]==ROCK) && cave->content[r][c]==EMPTY_CELL) {
        cave->content[r][c]=cave->content[r-1][c];
        cave->content[r-1][c]=EMPTY_CELL;
        /*related bitmaps are being updated.                                */
        display_cell(r, c , cave->content[r][c]);
        //fprintf(stderr,"%d %d:%c, ",r, c, cave->content[r][c]);
        display_cell(r-1, c , cave->content[r-1][c]);
        //fprintf(stderr,"%d %d:%c\n",r-1, c, cave->content[r-1][c]);
        //al_flip_display();

        //fprintf(stderr,"3:17;%c\n",cave->content[3][17]);
        //fprintf(stderr, "%d,%d:%c\n",r,c,cave->content[r][c]);
        //fprintf(stderr, "%d,%d:%c\n",r-1,c,cave->content[r-1][c]);
        //fprintf(stderr, "Somethings are being falled.\n");
    }
    //fprintf(stderr, "next row:%d\n",r+1);
    /*if there is somethings upperside                                      */
    if((r-2)>=0 && (cave->content[r-2][c]==ROCK || cave->content[r-2][c]== DIAMOND)){
        //fprintf(stderr,"control upper side.\n");
        control_falling(cave, r-2, c);
    }
    /*if the object is still falling.                                       */
    if((r+1)<(cave->dim_row) && cave->content[r+1][c]==EMPTY_CELL){
        // fprintf(stderr, "%d,%d- ",r+1,c);

        control_falling(cave, r+1, c);
        // for(i=0;i<cave->dim_row;++i){
        //     for(j=0;j<cave->dim_col;++j){
        //         //if(cave->content[i][j]==MINER)
        //         //    fprintf(stderr,"Miner's location is [%d,%d]\n",i,j);
        //         fprintf(stderr,"%c",cave->content[i][j]);
        //     }
        //     fprintf(stderr,"\n");
        // }
        // fprintf(stderr,"\n\n");
    }


    //al_update_display_region(r, c, CELL_SIZE, CELL_SIZE);
    //al_update_display_region(r-1, c, CELL_SIZE, CELL_SIZE);

    return;
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
                m->coord_r=r;
                m->coord_c=c;
            }
        }
    }
    //fprintf(stderr ,"in miner :%d,%d\n",r,c);
    return;
}
