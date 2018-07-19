#include<stdio.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_acodec.h"
#include"base.h"
#include"play.h"
#include"test.h"

#define MAP_FILE "maps.txt"

int
main()
{
    Game game;
    ALLEGRO_DISPLAY         *display;
    ALLEGRO_DISPLAY_MODE    disp_data;
    ALLEGRO_EVENT_QUEUE     *event_queue;
    ALLEGRO_SAMPLE          *sample;

    display=NULL;
    event_queue=NULL;
    game.head_cave=NULL;

    import_caves(&game,MAP_FILE);
    //test_import(&game);

    if(!al_init())
    {
        printf("Failed to initialize Allegro.\n");
        return -1;
    }

    al_get_display_mode(0, &disp_data);
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);

    display=al_create_display(disp_data.width, disp_data.height);
    if(!display)
    {
        printf("Failed to display\n");
        return -1;
    }

    if(!al_install_audio())
    {
        printf("Failed to initialize audio\n");
        return -1;
    }

    if(!al_init_acodec_addon())
    {
        printf("Failed to initialize audio codecs\n");
        return -1;
    }

    /*intro,    */
    if(!al_reserve_samples(1))
    {
        printf("Failed to reserve samples\n");
        return -1;
    }

    sample=al_load_sample("../audio/Music.wav");
    if(!sample)
    {
        printf("Intro music could not be loaded\n");
        return -1;
    }

    al_play_sample(sample,1.0,0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    /*
    event_queue=al_create_event_queue();
    if(!event_queue)
    {
        printf("Failed to create event_queue\n");
        al_destroy_display(display);
        return -1;
    }
    al_register_event_source(event_queue,al_get_display_event_source(display));
    */

    start_game(display,disp_data.width,disp_data.height);
    //arrange_bitmap_cells(display,&game);
    display_game(display,&game);

    if(display)
        al_destroy_display(display);
    if(sample)
        al_destroy_sample(sample);
    if(event_queue)
        al_destroy_event_queue(event_queue);

    free_caves(game.head_cave);
    return 0;
}
