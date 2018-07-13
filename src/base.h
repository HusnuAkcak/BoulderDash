
typedef struct Game
{
    char *cave_name;
    int score,max_time,diamond_num,diamond_value,extra_diamond_value;
    char **content;
    Game* next;
}Game;

void init_game(Game *game,char *map_file);
