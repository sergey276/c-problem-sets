#include "k.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE 4

#define GET_TILE(game, row, col) (game->board[row][col])

struct move_params {
    int row, col;
    bool is_horizontal;
    bool is_forward;
};

bool process_move(struct game *game, struct move_params params);
bool shift_tiles(struct game *game, struct move_params params, int *last_empty);
bool merge_tiles(struct game *game, struct move_params params, char *last_tile, int *last_index);
char* get_tile_ptr(struct game *game, struct move_params params, int index);
char upgrade_tile(char tile, int *score);
void add_random_tile(struct game *game);
bool is_move_possible(const struct game game);
bool update(struct game *game, int dy, int dx);
bool is_game_won(const struct game game);
void add_random_tile(struct game *game) {
        int row, col;
    while (1) {
        row = rand() % SIZE;
        col = rand() % SIZE;
        
        if (game->board[row][col] == ' ') {
            game->board[row][col] = (rand() & 1) ? 'B' : 'A';
            break;
        }
    }
}

bool is_move_possible(const struct game game) {
    int row = 0;
    while (row < SIZE) {  
        int col = 0;
        while (col < SIZE) {  
        
            if (game.board[row][col] == ' ') {
                return true;
            }

            char current = game.board[row][col];

            if (row > 0) { 
                if (game.board[row - 1][col] == current) { 
                    return true; 
                }
            }

            if (row < SIZE - 1) { 
                if (game.board[row + 1][col] == current) { 
                    return true; 
                }
            }

            if (col > 0) { 
                if (game.board[row][col - 1] == current) { 
                    return true; 
                }
            }

            if (col < SIZE - 1) { 
                if (game.board[row][col + 1] == current) { 
                    return true; 
                }
            }

            col++;
        }
        row++; 
    }
    return false;
}



bool process_move(struct game *game, struct move_params params) {
    bool moved = false;
    int i = 0;

    while (i < SIZE) {
        char last_tile = 0;
        int last_index = -1;
        int last_empty = -1;
        int j = params.is_forward ? SIZE - 1 : 0;

        while ((params.is_forward && j >= 0) || (!params.is_forward && j < SIZE)) {
            params.row = params.is_horizontal ? i : j;
            params.col = params.is_horizontal ? j : i;
            moved |= merge_tiles(game, params, &last_tile, &last_index);

            if (params.is_forward) {
                j--; 
            } else {
                j++;
            }
        }

        j = params.is_forward ? SIZE - 1 : 0;

        while ((params.is_forward && j >= 0) || (!params.is_forward && j < SIZE)) {
            params.row = params.is_horizontal ? i : j;
            params.col = params.is_horizontal ? j : i;
            moved |= shift_tiles(game, params, &last_empty);

            if (params.is_forward) {
                j--; 
            } else {
                j++; 
            }
        }

        i++; 
    }

    return moved;
}
bool update(struct game *game, int dy, int dx) {
  
    if ((dx != 0 && dy != 0)) { 
        return false;
    }
    if (dx == 0 && dy == 0) { 
        return false;
    }
    if (abs(dx) > 1 || abs(dy) > 1) { 
        return false;
    }

    struct move_params params;
params.is_horizontal = (dx != 0); 
params.is_forward = (dx > 0) || (dy > 0); 

params.row = dy;  
params.col = dx;

bool moved = process_move(game, params);

    if (!moved) {
        return false;
    }

    add_random_tile(game);
    return true;
}

bool is_game_won(const struct game game) {
    int row = 0;
    
    while (row < SIZE) {
        int col = 0;
        
        while (col < SIZE) {
          
            if (game.board[row][col] == 'K') {
                return true;
            }
            col++;
        }
        
        row++;
    }
    
    return false;
}


bool shift_tiles(struct game *game, struct move_params params, int *last_empty) {
  
    char *current_tile = &GET_TILE(game, params.row, params.col);

    if (*current_tile == ' ') {
    
        if (*last_empty < 0) {
            *last_empty = params.col;
        }
        return false;
    }
    if (*last_empty < 0) {
        return false;
    }

    char *target_tile = get_tile_ptr(game, params, *last_empty);

    *target_tile = *current_tile;
    *current_tile = ' ';

    *last_empty = params.col;

    return true;
}
bool merge_tiles(struct game *game, struct move_params params, char *last_tile, int *last_index) {
    
    char *current_tile = &GET_TILE(game, params.row, params.col);

    if (*current_tile == ' ') {
        return false;
    }

    if (*current_tile != *last_tile) {
        *last_tile = *current_tile;
        *last_index = params.col;
        return false;
    }

    *current_tile = ' ';
    GET_TILE(game, params.row, *last_index) = upgrade_tile(*last_tile, &game->score);
    
    *last_tile = 0;

    return true;
}



char* get_tile_ptr(struct game *game, struct move_params params, int index) {
    return &GET_TILE(game, params.row, index);
}

char upgrade_tile(char tile, int *score) {
    *score += 1 << (tile - 'A' + 2);
    return tile + 1;
}