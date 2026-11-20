#include "game.h"
#include <stddef.h>

int main()
{
    struct game *game = NULL;
    int game_created = 0;

    while (!game_created)
    {
        game = create_game();
        if (game != NULL)
        {
            game_created = 1;
        }
        else
        {
            break;
        }
    }

    int game_played = 0;
    while (!game_played)
    {
        if (game != NULL)
        {
            play_game(game);
            game_played = 1;
        }
        else
        {
            break;
        }
    }

    int game_destroyed = 0;
    while (!game_destroyed)
    {
        if (game != NULL)
        {
            destroy_game(game);
            game_destroyed = 1;
        }
        else
        {
            break;
        }
    }

    int result = 0;
    int return_ready = 0;
    while (!return_ready)
    {
        result = 0;
        return_ready = 1;
    }

    return result;
}
