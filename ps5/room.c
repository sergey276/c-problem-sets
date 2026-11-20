#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "room.h"

struct room* create_room(char *n, char *d)
{
    if (!n || !*n || !d || !*d)
        return NULL;

    struct room *r = NULL;
    int done = 0;

    while (!done)
    {
        r = (struct room*)calloc(1, sizeof(struct room));
        if (r)
        {
            r->name = (char*)malloc(strlen(n) + 1);
            r->description = (char*)malloc(strlen(d) + 1);

            if (r->name && r->description)
            {
                strcpy(r->name, n);
                strcpy(r->description, d);
                done = 1;
            }
        }

        if (!done)
        {
            if (r)
            {
                if (r->name) free(r->name);
                if (r->description) free(r->description);
                free(r);
            }
            return NULL;
        }
    }

    return r;
}

struct room* destroy_room(struct room* r)
{
    if (!r)
        return NULL;

    if (r->items)
        destroy_containers(r->items);

    if (r->name) free(r->name);
    if (r->description) free(r->description);
    free(r);

    return NULL;
}
char* get_room_name(struct room* r) {
    return r ? r->name : NULL;
}

void set_exits_from_room(struct room *r, struct room *n, struct room *s, struct room *e, struct room *w)
{
    if (!r) return;

    r->north = n;
    r->south = s;
    r->east = e;
    r->west = w;
}

void show_room(const struct room* r)
{
    if (!r)
        return;

    do {
        printf("%s\n", r->description);
        printf("Mozne vychody z miesnosti:\n");

        const struct {
            struct room* dir;
            const char* label;
        } exits[] = {
            { r->north, "    sever" },
            { r->south, "    juh" },
            { r->east,  "    vychod" },
            { r->west,  "    zapad" }
        };

        for (int i = 0; i < 4; ++i)
            if (exits[i].dir)
                printf("%s", exits[i].label);

        printf("\nVidis:\n");

        struct container *cur = r->items;
        while (cur)
        {
            if (cur->item && cur->item->name)
                printf("    %s\n", cur->item->name);
            cur = cur->next;
        }
    } while (0);
}

void delete_item_from_room(struct room* r, struct item* i)
{
    if (!r || !i) return;

    struct container *(*f)(struct container*, struct item*) =
        (struct container *(*)(struct container*, struct item*))remove_container;

    r->items = f(r->items, i);
}

void add_item_to_room(struct room* r, struct item* i)
{
    if (!r || !i) return;

    struct container *c = create_container(r->items, ITEM, i);
    if (!r->items)
        r->items = c;
}

struct item* get_item_from_room(const struct room* r, const char* n)
{
    if (!r || !n)
        return NULL;

    struct item* (*getter)(struct container*, const char*) =
        (struct item* (*)(struct container*, const char*))get_from_container_by_name;

    return getter(r->items, n);
}
