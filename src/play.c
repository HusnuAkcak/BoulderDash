#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

//this comment is in the eatsoil branch
void
start_game(Game *game,int scr_width,int scr_height)
{
    ALLEGRO_BITMAP          *image1;
    ALLEGRO_BITMAP          *image2;
    ALLEGRO_SAMPLE          *sample;

    /*intro music                                                      */
    al_reserve_samples(1);
    sample=al_load_sample(AUDIO_PATH"/Music.wav");
    al_play_sample(sample,1.0,0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

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

    display_game(game);

    al_destroy_bitmap(image1);
    al_destroy_bitmap(image2);
    al_destroy_sample(sample);
}

void
display_game(Game * g)
{
    Cave *curr_cave;
    char target;        /*target cell                                       */
    ALLEGRO_EVENT ev;
    bool finish;        /*if the game terminated or not.                    */

    finish=false;
    curr_cave=g->head_cave;
    display_curr_cave(curr_cave);
    while(!finish)
    {
        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            finish=true;
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
        {
            if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
                finish=true;
        /*
            if(ev.keyboard==ALLEGRO_DOWN)
                target=curr_cave[g->miner.coord_r-1][g->miner.coord_c];
            else if(ev.keyboard==ALLEGRO_UP)
                target=curr_cave[g->miner.coord_r+1][g->miner.coord_c];
            else if(ev.keyboard==ALLEGRO_KEY_LEFT)
                target=curr_cave[g->miner.coord_r][g->miner.coord_c-1];
            else if(ev.keyboard==ALLEGRO_KEY_RIGHT)
                target=curr_cave[g->miner.coord_r][g->miner.coord_c+1];

            if(target==SOIL)
        */

        }
        /*
        if(g->content[g->miner.coord_r][g->miner.coord_c]==GATE)
        {
            if(curr_cave->next!=NULL)
            {
                curr_cave=curr_cave->next;
            }
            else
            {
                finish=true;
            }
        }
        else if(g->content[g->miner.coord_r][g->miner.coord_c]==DIAMOND)
        {
            if(g->dia_req>0)
            {
                g->content[
            }
            else
            {

            }
        }
        */

    }


    return;
}

void
display_curr_cave(Cave * cave)
{
    int row,col;
    ALLEGRO_BITMAP *bitmap;

    if(!al_init_image_addon())
    {
        al_show_native_message_box(display,"Error","Error",
        "Failed to initialize al_init_image_addon",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }

    for(row=0;row<cave->dim_row;++row)
    {
        for(col=0;col<cave->dim_col;++col)
        {
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }


    for(row=0;row<cave->dim_row;++row)
    {
        for(col=0;col<cave->dim_col;++col)
        {
            switch (cave->content[row][col])
            {
                case EX_WALL:
                    bitmap=al_load_bitmap(IMG_PATH"/WALL.png");
                    break;
                case IN_WALL:
                    bitmap=al_load_bitmap(IMG_PATH"/wall.png");
                    break;
                case SOIL:
                    bitmap=al_load_bitmap(IMG_PATH"/soil.png");
                    break;
                case DIAMOND:
                    bitmap=al_load_bitmap(IMG_PATH"/diamond.png");
                    break;
                case ROCK:
                    bitmap=al_load_bitmap(IMG_PATH"/rock.png");
                    break;
                case MINER:
                    bitmap=al_load_bitmap(IMG_PATH"/miner.png");
                    break;
                case MONSTER:
                    bitmap=al_load_bitmap(IMG_PATH"/monster.png");
                    break;
                case GATE:
                    bitmap=al_load_bitmap(IMG_PATH"/gate.png");
                    break;
                case SNAKE:
                    bitmap=al_load_bitmap(IMG_PATH"/spider.png");
                    break;
                case WATER:
                    bitmap=al_load_bitmap(IMG_PATH"/water.png");
                    break;
            }

            al_draw_bitmap(bitmap, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            al_flip_display();
        }
    }
    al_destroy_bitmap(bitmap);
    return;
}
