#include<stdio.h>
#include<stdlib.h>
#include"allegro5/allegro.h"
#include"allegro5/allegro_image.h"
#include"allegro5/allegro_native_dialog.h"
#include"base.h"

void
string_cpy(char *dest, const char *src){

    int i;

    for(i=0;src[i]!=0;++i){
        dest[i]=src[i];
    }
    dest[i]=0;
    return;
}

int
string_cmp(const char * str1, const char *str2){

    int status; /*if strings are equal, status will be 0.                */
    int i;      /*loop variable                                          */

    status=0;
    i=0;
    while(status==0 && (str1[i]!=0 || str2[i]!=0)){
        if(str1[i]>str2[i]){
            status=1;
        }else if(str1[i]<str2[i]){
            status=-1;
        }
        ++i;
    }

    return status;
}

void
int_to_str(char *dest, int src){
    int i;
    if(src/10==0){
        for(i=0;dest[i]!=0;++i);
        dest[i]='0'+(src%10);
        dest[i+1]='\0';
        reverse_str(dest);
    }else{
        for(i=0;dest[i]!=0;++i);
        dest[i]='0'+(src%10);
        dest[i+1]='\0';
        int_to_str(dest, src/10);
    }
    return;
}

void
reverse_str(char *str){
    int i,str_len;
    char temp;

    for(str_len=0;str[str_len]!=0;++str_len);

    for(i=0; i<str_len/2; ++i){
        temp=str[i];
        str[i]=str[str_len-i-1];/*-1 comes from null character,at the end.*/
        str[str_len-i-1]=temp;
    }
    return;
}

void
string_cat(char* first, char *sec){
    int i;

    for(i=0;first[i]!=0;++i);
    first[i+1]=sec[0];

    return;
}

void
load_cave_bitmaps(){
    in_wall=al_load_bitmap(IMG_PATH"/in_wall.png");
    ex_wall=al_load_bitmap(IMG_PATH"/ex_wall.png");
    soil=al_load_bitmap(IMG_PATH"/soil.png");
    diamond=al_load_bitmap(IMG_PATH"/diamond.png");
    small_diamond=al_load_bitmap(IMG_PATH"/small_diamond.png");
    rock=al_load_bitmap(IMG_PATH"/rock.png");
    gate=al_load_bitmap(IMG_PATH"/gate.png");
    spider=al_load_bitmap(IMG_PATH"/spider.png");
    miner=al_load_bitmap(IMG_PATH"/miner.png");
    miner_symbol=al_load_bitmap(IMG_PATH"/miner_symbol.png");
    water=al_load_bitmap(IMG_PATH"/water.png");
    monster=al_load_bitmap(IMG_PATH"/monster.png");
    empty_cell=al_load_bitmap(IMG_PATH"/empty_cell.png");
    pause_button=al_load_bitmap(IMG_PATH"/pause.png");
    play_button=al_load_bitmap(IMG_PATH"/play.png");
    restart_level_button=al_load_bitmap(IMG_PATH"/restart_level.png");
    time_icon=al_load_bitmap(IMG_PATH"/time.png");
    explosion=al_load_bitmap(IMG_PATH"/explosion.png");

    return;
}

void
destroy_cave_bitmaps(){
    al_destroy_bitmap(in_wall);
    al_destroy_bitmap(ex_wall);
    al_destroy_bitmap(soil);
    al_destroy_bitmap(diamond);
    al_destroy_bitmap(small_diamond);
    al_destroy_bitmap(rock);
    al_destroy_bitmap(gate);
    al_destroy_bitmap(spider);
    al_destroy_bitmap(miner);
    al_destroy_bitmap(miner_symbol);
    al_destroy_bitmap(water);
    al_destroy_bitmap(monster);
    al_destroy_bitmap(empty_cell);
    al_destroy_bitmap(pause_button);
    al_destroy_bitmap(play_button);
    al_destroy_bitmap(restart_level_button);
    al_destroy_bitmap(time_icon);
    al_destroy_bitmap(explosion);

    return;
}

void
load_sample_instance(){

    al_reserve_samples(4);
    background=al_load_sample(AUDIO_PATH"/Music.wav");
    miner_dies=al_load_sample(AUDIO_PATH"/Explosion.wav");
    insect_dies=al_load_sample(AUDIO_PATH"/Explosion.wav");
    door_opens=al_load_sample(AUDIO_PATH"/Crack.wav");
    diamond_collect=al_load_sample(AUDIO_PATH"/Diamond.wav");
    eat_soil=al_load_sample(AUDIO_PATH"/MoveDirt.wav");
    rock_falling=al_load_sample(AUDIO_PATH"/Boulder.wav");

    background_instance=al_create_sample_instance(background);
    miner_dies_instance=al_create_sample_instance(miner_dies);
    insect_dies_instance=al_create_sample_instance(insect_dies);
    door_opens_instance=al_create_sample_instance(door_opens);
    diamond_collect_instance=al_create_sample_instance(diamond_collect);
    eat_soil_instance=al_create_sample_instance(eat_soil);
    rock_falling_instance=al_create_sample_instance(rock_falling);

    al_attach_sample_instance_to_mixer(background_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(miner_dies_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(insect_dies_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(door_opens_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(diamond_collect_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(eat_soil_instance, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(rock_falling_instance, al_get_default_mixer());

    al_set_sample_instance_playmode(background_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(miner_dies_instance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(insect_dies_instance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(door_opens_instance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(diamond_collect_instance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(eat_soil_instance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(rock_falling_instance, ALLEGRO_PLAYMODE_ONCE);

    return;
}

void
destroy_sample_instance(){

    return;
}

void
copy_point_array(Point *dest, Point *src, int size){

    int i;
    for(i=0; i<size; ++i){
        // fprintf(stderr, "(%d %d)\n", src[i].r, src[i].c);
        dest[i]=src[i];
        // fprintf(stderr, "{{%d %d}}\n", dest[i].r, dest[i].c);
    }

    return;
}

bool
is_pos_in_route(Point *curr_route, int curr_route_size, Point pos){
    bool in;
    int i;

    in=false;
    i=0;
    while(in==false && i<curr_route_size){
        if(curr_route[i].r==pos.r && curr_route[i].c==pos.c){
            // fprintf(stderr, "current detected repeted position is [%d %d]\n",pos.r, pos.c);
            in=true;
        }
        ++i;
    }

    return in;
}
