#include <stdlib.h>
#include <string.h>

#include "item.h"

char* get_item_name(struct item* it) {
    return it ? it->name : NULL;
}

struct item* create_item(char* name, char* description, unsigned int properties)
{
    struct item* result = NULL;
    int valid = 1;

    while (valid) {
        if (name == NULL) {
            break;
        }

        int is_empty = (name[0] == 0);
        if (is_empty) {
            break;
        }

        if (description == NULL) {
            break;
        }

        if (description[0] == 0) {
            break;
        }

        struct item* item = (struct item*)calloc(1, sizeof(struct item));
        if (item == NULL) {
            break;
        }

        item->name = (char*)malloc(strlen(name) + 1);
        if (item->name == NULL) {
            free(item);
            break;
        }

        item->description = (char*)malloc(strlen(description) + 1);
        if (item->description == NULL) {
            free(item->name);
            free(item);
            break;
        }

        item->properties = properties;

        int copied = 0;
        while (!copied) {
            strcpy(item->name, name);
            strcpy(item->description, description);
            copied = 1;
        }

        result = item;
        break;
    }

    return result;
}

struct item* destroy_item(struct item* item)
{
    while (1) {
        if (item == NULL) {
            break;
        }

        int done = 0;
        while (!done) {
            if (item->name != NULL) {
                free(item->name);
            }

            if (item->description != NULL) {
                free(item->description);
            }

            free(item);
            done = 1;
        }

        break;
    }

    return NULL;
}
