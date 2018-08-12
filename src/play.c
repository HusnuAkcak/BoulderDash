#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"cave.h"
#include"overhead.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
intro_game(Game *game,int scr_width,int scr_height){

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

    al_destroy_bitmap(image1);
    al_destroy_bitmap(image2);
    al_destroy_sample(sample);
}

void
play_game(Game * g){
    Cave curr_cave;
    ALLEGRO_EVENT ev;
    Direction dir;
    Point mouse_pos;
    bool play, moving;/*play :display screen, moving: miner moves.          */

    play=true;
    moving=true;
    g->status=CONTINUE;
    dir=NONE;
    curr_cave.content=NULL;
    curr_cave.dim_row=0;
    curr_cave.dim_col=0;
    copy_cave(&curr_cave, g->head_cave);
    find_miner_loc(&curr_cave, &(g->miner));    /*Miner's location is found. */
    while(g->status!=END){
        if(play && al_is_event_queue_empty(event_queue)){

            if(moving){
                al_clear_to_color(al_map_rgb(0,0,0));
                al_identity_transform(&camera);
                set_camera(g, &curr_cave);
                al_translate_transform(&camera, -(g->cam_pos.c), -(g->cam_pos.r));
                al_use_transform(&camera);
                moving=false;
            }
            display_curr_screen(&curr_cave, g);
            al_flip_display();


            /*this control is performed after al_flip_display, because if miner
             is dead, we freeze the screen for a while.                      */
            if(is_miner_dead(&curr_cave, &(g->miner))){
                --(g->miner.life);
                if((g->miner.life)>0){
                    restart_cave(g, &curr_cave);
                    moving=true;
                }else{
                    g->status=END;
                }
            }
            play=false;
        }

        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            g->status=END;
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE){
                g->status=END;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN){
                dir=DOWN;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
                dir=UP;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT){
                dir=LEFT;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT){
                dir=RIGHT;
            }
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_UP){
            dir=NONE;
        }
        else if(ev.type==ALLEGRO_EVENT_MOUSE_AXES){
            mouse_pos.c=ev.mouse.x;
            mouse_pos.r=ev.mouse.y;
        }
        else if(ev.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(mouse_pos.r>0 && mouse_pos.r<CELL_SIZE &&
                mouse_pos.c>(19*CELL_SIZE) &&
                mouse_pos.c<(20*CELL_SIZE)
            ){
                if(g->status==CONTINUE)
                    g->status=PAUSE;
                else if(g->status==PAUSE)
                    g->status=CONTINUE;
            }
        }
        else if(ev.type==ALLEGRO_EVENT_TIMER){

            if(ev.timer.source==main_timer){
                play=true;
            }
            if(ev.timer.source==panel_timer && g->status==CONTINUE){
                --curr_cave.max_time;
            }
            if(ev.timer.source==falling_timer && g->status==CONTINUE){
                control_falling(&curr_cave);
            }
            if(ev.timer.source==miner_timer && dir!=NONE && g->status==CONTINUE){
                g->status=move(&curr_cave, &(g->miner),dir);
                moving=true;
            }
        }
    }
    return;
}

Status
move(Cave * cave,Miner *m,Direction dir){

    Status status;      /*status of the game                                */
    char target;        /*target cell                                       */
    char after_target;  /*comes after target cell(in the same direction)    */
    Point tp, atp;      /*tp=target point, atp=after target point           */
    Point pre_pos;      /*previous position                                 */

    status=CONTINUE;/*status is set CONTINUE as default.                    */
    detect_target(dir, cave, m, &target, &after_target, &tp, &atp);

    /*it is controlled and if the move possible player's choice is applied. */
    if(target!=IN_WALL && target !=EX_WALL && target!=WATER &&
     !(target==ROCK && dir==UP) && !(target==ROCK && after_target!=EMPTY_CELL)){

        pre_pos=m->pos;
        m->pos=tp;      /*miner goes to target position(tp)                 */
        cave->content[tp.r][tp.c]=MINER;/*cave content is updated           */

        /*previous location(miner's old pos) is changed as EMPTY_CELL       */
        cave->content[pre_pos.r][pre_pos.c]=EMPTY_CELL;

        if(cave->content[tp.r][tp.c]==GATE){/*this function will be adjusted then.*/
            if((cave->dia_req)-(m->collected_dia)<=0 && cave->next!=NULL){
                cave=cave->next;
            }
            else if((cave->dia_req)-(m->collected_dia)<=0 && cave->next==NULL){
                status=END;
            }
        }
        else if(target==DIAMOND){
            ++m->collected_dia;
            if((cave->dia_req)-(m->collected_dia)>0){
                m->curr_cave_score+=cave->dia_val;
                m->score+=cave->dia_val;    /*general score                 */
            }
            else{
                m->curr_cave_score+=cave->ex_dia_val;
                m->score+=cave->ex_dia_val; /*general score                 */
            }
        }
        else if(target==ROCK && after_target==EMPTY_CELL){
            if(dir!=UP){
                cave->content[atp.r][atp.c]=ROCK;
            }
        }

        display_cell(pre_pos, cave);
        display_cell(tp, cave);
        display_cell(atp, cave);
    }


    return status;
}
