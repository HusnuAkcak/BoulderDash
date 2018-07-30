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
start_game(Game *game,int scr_width,int scr_height){
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
display_cell(int row, int col,Content content){
    ALLEGRO_BITMAP *bitmap;

    switch(content){
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
        case SPIDER:
            bitmap=al_load_bitmap(IMG_PATH"/spider.png");
            break;
        case WATER:
            bitmap=al_load_bitmap(IMG_PATH"/water.png");
            break;
        case EMPTY_CELL:
            bitmap=al_load_bitmap(IMG_PATH"/empty_cell.png");
            break;
        }

        al_draw_bitmap(bitmap, col*CELL_SIZE, CELL_SIZE+(row*CELL_SIZE), 0);
        al_flip_display();
        al_destroy_bitmap(bitmap);
}

bool
move(Cave * curr_cave,Miner *m,Direction dir){
    char target;/*target cell               ]                               */
    int r,c;    /*row and column                                            */
    bool cont;  /*continue                                                  */

    cont=true;

    /*target is determined                                                  */
    if(dir==UP){
        target=curr_cave->content[m->coord_r-1][m->coord_c];
    }else if(dir==RIGHT){
        target=curr_cave->content[m->coord_r][m->coord_c+1];
    }else if(dir==DOWN){
        target=curr_cave->content[m->coord_r+1][m->coord_c];
    }else if(dir==LEFT){
        target=curr_cave->content[m->coord_r][m->coord_c-1];
    }

    /*it is controlled and if the move possible player's choice is applied. */
    if(target!=IN_WALL && target !=EX_WALL && target!=ROCK){
        curr_cave->content[m->coord_r][m->coord_c]=EMPTY_CELL;
        display_cell(m->coord_r, m->coord_c,EMPTY_CELL);

        if(dir==UP){
            //curr_cave->content[m->coord_r-1][m->coord_c]=MINER;
            m->coord_r=m->coord_r-1;
        }else if(dir==RIGHT){
            //curr_cave->content[m->coord_r][m->coord_c+1]=MINER;
            m->coord_c=m->coord_c+1;
        }else if(dir==DOWN){
            //curr_cave->content[m->coord_r+1][m->coord_c]=MINER;
            m->coord_r=m->coord_r+1;
        }else if(dir==LEFT){
            //curr_cave->content[m->coord_r][m->coord_c-1]=MINER;
            m->coord_c=m->coord_c-1;
        }

        if(curr_cave->content[m->coord_r][m->coord_c]==GATE){
            if(curr_cave->dia_req<=0 && curr_cave->next!=NULL){
                curr_cave=curr_cave->next;
                /*For new level, the miner's location is found.                 */
                for(r=0;r<curr_cave->dim_row;++r){
                    for(c=0;c<curr_cave->dim_col;++c){
                        if(curr_cave->content[r][c]==MINER){
                            m->coord_r=r;
                            m->coord_c=c;
                        }
                    }
                }
            }
            else if(curr_cave->dia_req<=0 && curr_cave->next==NULL){
                cont=false;
            }
        }
        else if(curr_cave->content[m->coord_r][m->coord_c]==SOIL ||
                    curr_cave->content[m->coord_r][m->coord_c]==' '){
            curr_cave->content[m->coord_r][m->coord_c]=MINER;
            display_cell(m->coord_r, m->coord_c,EMPTY_CELL);
            display_cell(m->coord_r, m->coord_c, MINER);
        }
    }

    return cont;
}

void
display_game(Game * g)
{
    Cave *curr_cave;
    ALLEGRO_EVENT ev;
    int i,j;
    bool cont;  /*continue                                                */

    curr_cave=g->head_cave;
    cont=true;
    /*Miner's location is found.                                          */
    for(i=0;i<curr_cave->dim_row;++i){
        for(j=0;j<curr_cave->dim_col;++j){
            if(curr_cave->content[i][j]==MINER){
                g->miner.coord_r=i;
                g->miner.coord_c=j;
            }
        }
    }

    display_curr_cave(curr_cave);
    while(cont){
        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            cont=false;
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE){
                cont=false;
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN){
                cont=move(curr_cave,&(g->miner),DOWN);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
                cont=move(curr_cave,&(g->miner),UP);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT){
                cont=move(curr_cave,&(g->miner),LEFT);
            }
            else if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT){
                cont=move(curr_cave,&(g->miner),RIGHT);
            }
        }
    }
    return;
}

void
display_curr_cave(Cave * cave){
    int row,col;

    if(!al_init_image_addon()){
        al_show_native_message_box(display,"Error","Error",
        "Failed to initialize al_init_image_addon",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }

    al_clear_to_color(al_map_rgb(0,0,0));
    for(row=0;row<cave->dim_row;++row){
        for(col=0;col<cave->dim_col;++col){
            display_cell(row, col, cave->content[row][col]);
        }
    }
    return;
}
