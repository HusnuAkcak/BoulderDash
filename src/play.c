#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"cave.h"
#include"overhead.h"
#include"insect.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

void
intro_game(Game *game,int scr_width,int scr_height){

    ALLEGRO_BITMAP      *image1;
    ALLEGRO_BITMAP      *image2;


    /*background music(It is always played.)                               */
    al_play_sample_instance(background_instance);

    /*First intro image                                                    */
    image1=al_load_bitmap(IMG_PATH"/boulder_dash.jpg");
    al_draw_bitmap(image1,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.1);
    al_clear_to_color(al_map_rgb(0,0,0));
    /*Second intro image                                                    */
    image2=al_load_bitmap(IMG_PATH"/title_screen.png");
    al_draw_bitmap(image2,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.1);
    al_clear_to_color(al_map_rgb(0,0,0));

    al_destroy_bitmap(image1);
    al_destroy_bitmap(image2);
}

void
play_game(Game * g){
    Cave curr_cave;
    ALLEGRO_EVENT ev;
    Point mouse_pos;
    bool play, moving;/*play :display screen, moving: miner moves.          */

    /*Miner's initial values.                                               */
    g->miner.life=MINER_LIFE;
    g->miner.alive=true;
    g->miner.score=0;
    g->miner.curr_cave_score=0;
    g->miner.collected_dia=0;

    play=true;              /*if play==true, the screen will be refreshed.   */
    moving=true;            /*when miner moves, it turns out true.           */
    g->status=CONTINUE;     /*status is initialized as CONTINUE.             */
    g->miner.move_dir=NONE; /*miner's movement direction                     */

    /*current cave's initial values.                                         */
    curr_cave.content=NULL;
    curr_cave.head_monster=NULL;
    curr_cave.head_spider=NULL;
    curr_cave.dim_row=0;
    curr_cave.dim_col=0;

    copy_cave(&curr_cave, g->head_cave);
    g->miner.duration_of_death=MINER_DEATH_DURATION;
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

            if(g->status==NEXT){
                if(curr_cave.left_time>0){//left time is being added to miner's score.
                    --curr_cave.left_time;
                    ++(g->miner.score);
                    ++(g->miner.curr_cave_score);
                }
                else{
                    g->status=go_next_cave(g, &curr_cave);
                    moving=true;
                }
            }

            display_curr_screen(&curr_cave, g);
            al_flip_display();

            /*this control is performed after al_flip_display, because if miner
            is dead, we freeze the screen for a while.                      */
            if(g->miner.alive==false){
                al_play_sample_instance(miner_dies_instance);
                g->status=PAUSE;
                if((g->miner.duration_of_death)<=0){
                    --(g->miner.life);
                    if((g->miner.life)>0){
                        restart_cave(g, &curr_cave);
                        moving=true;
                    }else{
                        g->status=END;
                    }
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
                g->miner.move_dir=DOWN;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
                g->miner.move_dir=UP;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT){
                g->miner.move_dir=LEFT;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT){
                g->miner.move_dir=RIGHT;
            }
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_UP){
            g->miner.move_dir=NONE;
        }
        else if(ev.type==ALLEGRO_EVENT_MOUSE_AXES){
            mouse_pos.c=ev.mouse.x;
            mouse_pos.r=ev.mouse.y;
        }
        else if(ev.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(mouse_pos.r>0 && mouse_pos.r<CELL_SIZE &&
                mouse_pos.c>(18*CELL_SIZE) &&
                mouse_pos.c<(19*CELL_SIZE)
            ){
                if(g->status==CONTINUE){
                    g->status=PAUSE;
                    al_stop_sample_instance(background_instance);
                }
                else if(g->status==PAUSE){
                    g->status=CONTINUE;
                    al_play_sample_instance(background_instance);
                }
            }
            else if(mouse_pos.r>0 && mouse_pos.r<CELL_SIZE &&
                mouse_pos.c>(19*CELL_SIZE) &&
                mouse_pos.c<(20*CELL_SIZE)
            ){
                al_stop_sample_instance(background_instance);
                al_play_sample_instance(background_instance);
                restart_cave(g, &curr_cave);
            }
        }
        else if(ev.type==ALLEGRO_EVENT_TIMER){

            if(ev.timer.source==main_timer){
                is_miner_dead(g, &curr_cave, &(g->miner));
                play=true;
            }

            if(ev.timer.source==panel_timer && g->status==CONTINUE){
                --curr_cave.left_time;

                if( (curr_cave.left_time) < (curr_cave.max_time/10.0) ){
                    /*in last 1/10 time left, music volume is inreased.*/
                    al_set_sample_instance_gain(background_instance, 3);
                }
            }
            else if(ev.timer.source==panel_timer && (g->miner.alive==false) ){
                /*left time is added to score.                              */
                --(g->miner.duration_of_death);
            }

            if(ev.timer.source==falling_timer && g->status==CONTINUE){
                move_insects(g, &curr_cave);
                control_falling(&(g->miner), &curr_cave);
            }

            if(ev.timer.source==miner_timer && g->miner.move_dir!=NONE && g->status==CONTINUE){
                g->status=move(&curr_cave, &(g->miner));
                moving=true;
            }
        }


    }
    return;
}

Status
move(Cave * cave,Miner *m){

    Status status;      /*status of the game                                */
    char target;        /*target cell                                       */
    char after_target;  /*comes after target cell(in the same direction)    */
    Point tp, atp;      /*tp=target point, atp=after target point           */
    Point pre_pos;      /*previous position                                 */

    status=CONTINUE;/*status is set CONTINUE as default.                    */
    detect_target(cave, m, &target, &after_target, &tp, &atp);

    /*it is controlled and if the move possible player's choice is applied. */
    if(target!=IN_WALL && target !=EX_WALL && target!=WATER &&
    !(target==ROCK && m->move_dir==UP) && !(target==ROCK && after_target!=EMPTY_CELL)){

        pre_pos=m->pos;
        m->pos=tp;      /*miner goes to target position(tp)                 */
        cave->content[tp.r][tp.c]=MINER;/*target is occupied.               */

        /*previous location(miner's old pos) is changed as EMPTY_CELL       */
        cave->content[pre_pos.r][pre_pos.c]=EMPTY_CELL;

        if(target==GATE){
            if((cave->dia_req)-(cave->collected_dia)<=0){
                status=NEXT;
            }
        }
        else if(target==DIAMOND){
            ++m->collected_dia;
            ++cave->collected_dia;
            if(cave->dia_req==cave->collected_dia){
                al_play_sample_instance(door_opens_instance);
                cave->content[cave->gate_loc.r][cave->gate_loc.c]=GATE;
            }

            if((cave->dia_req)-(cave->collected_dia)>0){
                m->curr_cave_score+=cave->dia_val;
                m->score+=cave->dia_val;    /*general score                 */
            }
            else{
                m->curr_cave_score+=cave->ex_dia_val;
                m->score+=cave->ex_dia_val; /*general score                 */
            }
        }
        else if(target==ROCK && after_target==EMPTY_CELL){
            if(m->move_dir!=UP){
                cave->content[atp.r][atp.c]=ROCK;
            }
        }
    }


    return status;
}
