#include <stdlib.h>
#include <strings.h>

#include "game.h"
#include "world.h"
#include "parser.h"
#include "room.h"
#include "item.h"
#include "backpack.h"
#include "command.h"

struct game* create_game()
{
    struct game* game = (struct game*)calloc(1, sizeof(struct game));

    if (game != NULL) {
        game->state = PLAYING;
        game->parser = create_parser();
        game->world = create_world();
        game->backpack = create_backpack(5);

        if (game->world != NULL) {
            game->current_room = game->world->room;
        }
    }

    return game;
}

struct game* destroy_game(struct game* game)
{
    if (game == NULL) {
        return NULL;
    }

    destroy_parser(game->parser);
    destroy_world(game->world);
    destroy_backpack(game->backpack);

    free(game);
    return NULL;
}

void execute_command(struct game* game, struct command* command)
{
    if (game == NULL || command == NULL) {
        return;
    }

    if (!strcasecmp(command->name, "KONIEC")) {
        game->state = GAMEOVER;
        return;
    }

    if (!strcasecmp(command->name, "SEVER") && game->current_room->north != NULL) {
        game->current_room = game->current_room->north;
        return;
    }

    if (!strcasecmp(command->name, "JUH") && game->current_room->south != NULL) {
        game->current_room = game->current_room->south;
        return;
    }

    if (!strcasecmp(command->name, "VYCHOD") && game->current_room->east != NULL) {
        game->current_room = game->current_room->east;
        return;
    }

    if (!strcasecmp(command->name, "ZAPAD") && game->current_room->west != NULL) {
        game->current_room = game->current_room->west;
        return;
    }

    if (!strcasecmp(command->name, "ROZHLIADNI SA")) {
        show_room(game->current_room);
        return;
    }

    if (!strcasecmp(command->name, "RESTART")) {
        game->state = RESTART;
        return;
    }
}

void play_game(struct game* game)
{
    if (game == NULL)
        return;

    struct item* i_keycard = get_item_from_room(game->current_room, "Keycard");

    if (i_keycard != NULL) {
        if (add_item_to_backpack(game->backpack, i_keycard)) {
            struct item* fetched = get_item_from_backpack(game->backpack, "Keycard");

            if (fetched != NULL) {
                delete_item_from_backpack(game->backpack, fetched);
                delete_item_from_room(game->current_room, fetched);
                destroy_item(fetched);
            }
        }
    }

    struct command* command = parse_input(game->parser, "RESTART");
    if (command != NULL) {
        execute_command(game, command);
        destroy_command(command);
    }

    struct room* room = get_room(game->world, "Airlock");
    if (room != NULL) {
        show_room(room);
    }
}
