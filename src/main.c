#include<stdio.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_acodec.h"
#include"allegro5/allegro_audio.h"
#include"allegro5/allegro_ttf.h"
#include"base.h"
#include"cave.h"
#include"play.h"
#include"test.h"

int
main()
{
    Game game;                      /*game struct                           */
    ALLEGRO_DISPLAY_MODE disp_data; /*frame width and height is keept.      */

    display=NULL;
    event_queue=NULL;

    import_caves(&game);
    //test_import(&game);//could be called when it is wanted to see if the maps are correcty imported or not.


    /*Initialisations                                                       */
    al_init();
    al_install_audio();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_init_image_addon();


    /*Mouse and keyboard options are installed.                             */
    al_install_mouse();
    al_install_keyboard();


    /*Screen width and height informations are recieved with 'disp_data'    */
    al_get_display_mode(0, &disp_data);

    main_timer=al_create_timer(1.0/FPS);/*timer for game operations         */
    panel_timer=al_create_timer(1.0);/*timer for score panel                */
    falling_timer=al_create_timer(1.0/FALL_PS);/*timer for falling obj.     */
    miner_timer=al_create_timer(1.0/MPS);
    event_queue=al_create_event_queue();/*Event queue is created.           */

    /*The font is loaded.                                                   */
    font=al_load_font(FONT_FILE,FONT_SIZE,0);

    /*The frame is displayed on the screen.                                 */
    display=al_create_display(disp_data.width, disp_data.height);

    /*The components are introduced to the event queue                      */
    al_register_event_source(event_queue,al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_timer_event_source(main_timer));
    al_register_event_source(event_queue,al_get_timer_event_source(panel_timer));
    al_register_event_source(event_queue,al_get_timer_event_source(falling_timer));
    al_register_event_source(event_queue,al_get_timer_event_source(miner_timer));
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_keyboard_event_source());

    al_start_timer(main_timer);
    al_start_timer(panel_timer);
    al_start_timer(falling_timer);
    al_start_timer(miner_timer);

    load_cave_bitmaps();    //Cave bitmaps are loaded.      
    load_sample_instance(); //samples and sample instances are loaded.
    /*The game structure is started.                                        */
    intro_game(&game,disp_data.width,disp_data.height);
    play_game(&game);

    /*######################### Free operations ############################*/

    free_caves(game.head_cave); // Caves are freed.
    destroy_cave_bitmaps();     // Cave bitmaps are freed.

    /*samples are freed.                                                    */
    al_destroy_sample(background);
    al_destroy_sample_instance(background_instance);

    /*Other free opererations.                                              */
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_timer(main_timer);

    return 0;
}
