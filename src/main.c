#include<stdio.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_ttf.h"
#include"base.h"
#include"play.h"
#include"test.h"

int
main()
{
    Game game;
    ALLEGRO_DISPLAY_MODE disp_data;

    display=NULL;
    event_queue=NULL;

    import_caves(&game);
    /*test_import(&game);//could be called when it is wanted to see if the maps are correcty imported or not.
    */

    /*initialisations                                                   */
    al_init();
    al_install_audio();
    al_init_font_addon();
    al_install_mouse();
    al_install_keyboard();
    al_get_display_mode(0, &disp_data);
    al_init_acodec_addon();
    al_init_image_addon();

    font=al_load_font(FONT_FILE,FONT_SIZE,0);
    timer=al_create_timer(1/FPS);
    display=al_create_display(disp_data.width, disp_data.height);
    event_queue=al_create_event_queue();

    al_register_event_source(event_queue,al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_keyboard_event_source());

    start_game(&game,disp_data.width,disp_data.height);

    /*free operations                                                       */
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_timer(timer);
    free_caves(game.head_cave);
    return 0;
}
