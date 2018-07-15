#include <stdio.h>
#include "allegro5/allegro.h"
#include "base.h"
#include "play.h"
#include "test.h"

#define MAP_FILE "maps.txt"

int
main()
{
    Game game;
    ALLEGRO_DISPLAY         *display;
    ALLEGRO_DISPLAY_MODE    disp_data;

    game.head_cave=NULL;
    import_caves(&game,MAP_FILE);
    test_import(&game);

    display=NULL;
    if(!al_init())
    {
        printf("Failed to initialize Allegro.\n");
    }

    al_get_display_mode(0,&disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display=al_create_display(disp_data.width, disp_data.height);

    al_destroy_display(display);
    free_cave(game.head_cave);
    return 0;
}
