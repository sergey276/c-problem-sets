#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "k.h"
#include "hof.h"
#include "ui.h"

int main() {
    struct player records[10];
    struct player current_player = { .name = "manager", .score = 1500 };
    
    int total = load(records);
    add_player(records, &total, current_player);
    save(records, total);

    struct game game = {
        .board = {
            {'A', ' ', ' ', ' '},
            {'B', ' ', ' ', 'B'},
            {'C', 'C', 'C', ' '},
            {'D', 'D', 'D', 'D'}
        },
        .score = 0
    };

    add_random_tile(&game);
    
    if (update(&game, 0, 1))
        printf("Move successful!\n");
    else
        printf("Move failed!\n");

    if (is_game_won(game))
        printf("Congratulations, you've won!\n");

    if (!is_move_possible(game))
        printf("No more moves possible. Game over!\n");

    render(game);

    return 0;
}
