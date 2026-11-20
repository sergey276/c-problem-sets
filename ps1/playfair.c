#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "playfair.h"
#include <stdbool.h>
#define MATRIX_SIZE 5
#define PLACEHOLDER 'X'
#define SUBSTITUTION_FROM 'W'
#define SUBSTITUTION_TO 'V'

bool fill_key_array(char key_arr[], const char *source);

void pair_handle(const char key_arr[], char pair[2], bool encrypt);
void get_coords_from_letter(const char key_arr[], char letter, int coords[2]);

void get_letter_from_coords(const char key_arr[], char *letter, const int coords[2]);
void adjust_coord(int *coord, int shift);

int find_next_valid_char(const char *text, int length, int start_index);

char* playfair_encrypt(const char* key, const char* text) {
    if (!key || !text) return NULL;

    int text_length = strlen(text);
    char *result = malloc(text_length * 3);
    if (!result) return NULL;

    char key_arr[26] = {0};
    if (!fill_key_array(key_arr, key)) {
        free(result);
        return NULL;
    }
    fill_key_array(key_arr, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    char *out_ptr = result;
    for (int i = 0; i < text_length;) {
        int first = find_next_valid_char(text, text_length, i);
        if (first < 0) break;
        int second = find_next_valid_char(text, text_length, first + 1);

        out_ptr[0] = toupper(text[first]);
        if (out_ptr[0] == SUBSTITUTION_FROM) out_ptr[0] = SUBSTITUTION_TO;

        if (second >= 0) {
            out_ptr[1] = toupper(text[second]);
            if (out_ptr[1] == SUBSTITUTION_FROM) out_ptr[1] = SUBSTITUTION_TO;

            if (out_ptr[0] == out_ptr[1]) {
                out_ptr[1] = PLACEHOLDER;
                i = first + 1;
            } else {
                i = second + 1;
            }
        } else {
            out_ptr[1] = PLACEHOLDER;
            i = first + 1;
        }

        pair_handle(key_arr, out_ptr, true);
        out_ptr += 2;
    }
    *out_ptr = '\0';
    return result;
}

char* playfair_decrypt(const char* key, const char* text) {
    if (!key || !text) return NULL;

    int text_length = strlen(text);
    char *result = malloc(text_length + 1);
    if (!result) return NULL;

    char key_arr[26] = {0};
    if (!fill_key_array(key_arr, key)) {
        free(result);
        return NULL;
    }
    fill_key_array(key_arr, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    char *out_ptr = result;
    for (int i = 0; i < text_length; i += 2) {
        if (i + 1 >= text_length) break;
        out_ptr[0] = toupper(text[i]);
        out_ptr[1] = toupper(text[i + 1]);

        pair_handle(key_arr, out_ptr, false);
        out_ptr += 2;
    }
    *out_ptr = '\0';
    return result;
}


void pair_handle(const char key_arr[], char pair[2], bool encrypt) {
    int coords[2][2];
    for (int i = 0; i < 2; ++i)
        get_coords_from_letter(key_arr, pair[i], coords[i]);

    if (coords[0][1] == coords[1][1]) {
        for (int i = 0; i < 2; ++i)
            adjust_coord(&coords[i][0], encrypt ? 1 : -1);
    } else if (coords[0][0] == coords[1][0]) {
        for (int i = 0; i < 2; ++i)
            adjust_coord(&coords[i][1], encrypt ? 1 : -1);
    } else {
        int temp = coords[0][1];
        coords[0][1] = coords[1][1];
        coords[1][1] = temp;
    }

    for (int i = 0; i < 2; ++i)
        get_letter_from_coords(key_arr, &pair[i], coords[i]);
}

bool fill_key_array(char key_arr[], const char *source) {
    int key_length = 0;
    while (*source) {
        char letter = toupper(*source++);
        if (letter < 'A' || letter > 'Z') continue;
        if (letter == SUBSTITUTION_FROM) letter = SUBSTITUTION_TO;
        if (!strchr(key_arr, letter)) key_arr[key_length++] = letter;
        if (key_length >= 25) break;
    }
    key_arr[key_length] = '\0';
    return key_length > 0;
}



void get_letter_from_coords(const char key_arr[], char *letter, const int coords[2]) {
    *letter = key_arr[coords[0] * MATRIX_SIZE + coords[1]];
}


void get_coords_from_letter(const char key_arr[], char letter, int coords[2]) {
    for (int i = 0; i < 25; ++i)
        if (key_arr[i] == letter) {
            coords[0] = i / MATRIX_SIZE;
            coords[1] = i % MATRIX_SIZE;
            return;
        }
}

int find_next_valid_char(const char *text, int length, int start_index) {
    while (start_index < length && text[start_index] == ' ')
        start_index++;
    return (start_index < length) ? start_index : -1;
}
void adjust_coord(int *coord, int shift) {
    *coord = (*coord + shift + MATRIX_SIZE) % MATRIX_SIZE;
}