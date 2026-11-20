#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "k.h"

void render(const struct game game) {
    clear();
    char buffer[20];
    sprintf(buffer, "Score: %d", state.score);
    attrset(COLOR_PAIR(0));
    mvprintw(1, (COLS - (int)strlen(buffer)) / 2, buffer);
    
    int x_offset = (COLS - 25) / 2;
    int y_offset = 3;
    attrset(COLOR_PAIR(6));
    
    int j = y_offset;
    while (j < y_offset + 17) {
        int i = x_offset;
        while (i < x_offset + 25) {
            mvprintw(j, i, " ");
            i++;
        }
        j++;
    }

    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            char cell = state.board[row][col];
            
            if (cell == ' ')
                attrset(COLOR_PAIR(0));
            else if (cell < 'C')
                attrset(COLOR_PAIR(1));
            else if (cell < 'E')
                attrset(COLOR_PAIR(2));
            else if (cell < 'G')
                attrset(COLOR_PAIR(3));
            else if (cell < 'J')
                attrset(COLOR_PAIR(4));
            else
                attrset(COLOR_PAIR(5));
            
            int y_pos = y_offset + (row * SIZE) + 1;
            int x_pos = x_offset + (col * 6) + 1;
            
            mvprintw(y_pos, x_pos, "     ");
            mvprintw(y_pos + 1, x_pos, "  %c  ", cell);
            mvprintw(y_pos + 2, x_pos, "     ");
            
            col++;
        }
        row++;
    }
    
    refresh();
}