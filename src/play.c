#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"

#define CELL_SIZE 32    /*  px  */
void
start_game(ALLEGRO_DISPLAY *display,int scr_width,int scr_height)
{
    ALLEGRO_BITMAP          *image1;
    ALLEGRO_BITMAP          *image2;

    if(!al_init_image_addon())
    {
        al_show_native_message_box(display,"Error","Error",
        "Failed to initialize al_init_image_addon",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }

    image1=al_load_bitmap("../img/boulder_dash.jpg");
    if(!image1)
    {
        al_show_native_message_box(display,"Error","Error",
        "Failed to load image",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }
    al_draw_bitmap(image1,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.5);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_destroy_bitmap(image1);

    image2=al_load_bitmap("../img/title_screen.png");
    if(!image2)
    {
        al_show_native_message_box(display,"Error","Error",
        "Failed to load image",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }
    al_draw_bitmap(image2,scr_width/3,scr_height/3,0);
    al_flip_display();
    al_rest(0.5);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_destroy_bitmap(image2);
}

void
display_game(ALLEGRO_DISPLAY *display,Game * game)
{
    Cave *curr_cave;

    for(curr_cave=game->head_cave; curr_cave!=NULL; curr_cave=curr_cave->next)
    {
        display_curr_cave(display,curr_cave);
        al_rest(1);
    }
    return;
}

void
display_curr_cave(ALLEGRO_DISPLAY *display,Cave * cave)
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
            if(cave->content[row][col]=='W')
            {
                bitmap=al_load_bitmap("../img/WALL.png");
            }
            else if(cave->content[row][col]=='w')
            {
                bitmap=al_load_bitmap("../img/wall.png");
            }
            else if(cave->content[row][col]=='.')
            {
                bitmap=al_load_bitmap("../img/soil.png");
            }
            else if(cave->content[row][col]=='d')
            {
                bitmap=al_load_bitmap("../img/diamond.png");
            }
            else if(cave->content[row][col]=='r')
            {
                bitmap=al_load_bitmap("../img/rock.png");
            }
            else if(cave->content[row][col]=='P')
            {
                bitmap=al_load_bitmap("../img/miner.png");
            }
            else if(cave->content[row][col]=='X')
            {
                bitmap=al_load_bitmap("../img/gate.png");
            }
            else if(cave->content[row][col]=='M')
            {
                bitmap=al_load_bitmap("../img/monster.png");
            }
            else if(cave->content[row][col]=='S')
            {
                bitmap=al_load_bitmap("../img/spider.png");
            }
            else if(cave->content[row][col]=='l')
            {
                bitmap=al_load_bitmap("../img/water.png");
            }

            al_draw_bitmap(bitmap, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
            al_flip_display();
        }
    }
    return;
}
