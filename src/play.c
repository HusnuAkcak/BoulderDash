#include<stdio.h>
#include<stdlib.h>
#include"base.h"
#include"play.h"
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"


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
    al_rest(1);
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
    al_rest(1);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_destroy_bitmap(image2);
}


void
display_curr_level(Game *game)
{

    return;
}
