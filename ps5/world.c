#include <stdlib.h>
#include <strings.h>

#include "world.h"

static struct room* __internal_register(struct container* w, const char* n, const char* d);

struct container* create_world()
{
    struct room *r0 = create_room("Entrance", "Entrance");
    struct container *w = create_container(NULL, ROOM, r0);

    struct {
        struct room **target;
        const char *name;
        const char *desc;
    } rooms[] = {
        { &r0, "Entrance", "Entrance" },
        { NULL, "Airlock", "Airlock" },
        { NULL, "Hallway - 1", "Hallway - 1" },
        { NULL, "Hallway - 2", "Hallway - 2" },
        { NULL, "Living quarters", "Living quarters" },
        { NULL, "Kitchen", "Kitchen" },
        { NULL, "WC", "WC" },
        { NULL, "Command center", "Command center" },
        { NULL, "Missile silo access", "Missile silo access" },
        { NULL, "Communication center", "Communication center" },
        { NULL, "Infirmary", "Infirmary" },
        { NULL, "Storage", "Storage" },
        { NULL, "Technical room", "Technical room" },
        { NULL, "Air purification unit", "Air purification unit" },
        { NULL, "Water purification unit", "Water purification unit" },
    };

    struct room* r[15];
    for (int i = 1; i < 15; ++i)
        r[i] = __internal_register(w, rooms[i].name, rooms[i].desc);

    set_exits_from_room(r0, r[1], NULL, NULL, NULL);
    set_exits_from_room(r[1], r[2], r0, NULL, NULL);
    set_exits_from_room(r[2], r[3], r[1], r[7], r[4]);
    set_exits_from_room(r[3], r[11], r[2], r[10], r[9]);
    set_exits_from_room(r[4], NULL, r[5], r[2], r[6]);
    set_exits_from_room(r[5], r[4], NULL, NULL, NULL);
    set_exits_from_room(r[6], NULL, r[4], NULL, NULL);
    set_exits_from_room(r[7], NULL, NULL, r[8], r[2]);
    set_exits_from_room(r[8], NULL, NULL, NULL, r[7]);
    set_exits_from_room(r[9], NULL, NULL, r[3], NULL);
    set_exits_from_room(r[10], NULL, NULL, NULL, r[3]);
    set_exits_from_room(r[11], NULL, r[3], NULL, r[12]);
    set_exits_from_room(r[12], r[14], NULL, r[11], r[13]);
    set_exits_from_room(r[13], NULL, NULL, r[12], NULL);
    set_exits_from_room(r[14], NULL, r[12], NULL, NULL);

    struct {
        struct room* r;
        const char* n;
        const char* d;
        int flags;
    } items[] = {
        { r0, "Keycard", "Keycard", MOVABLE | USABLE },
        { r[11], "Filter", "Air filter", MOVABLE | USABLE },
        { r[6], "Toothbrush", "Toothbrush", MOVABLE },
        { r[9], "Paper with codes", "Paper with codes", MOVABLE | EXAMINABLE | USABLE },
        { r[10], "First aid kit", "First aid kit", MOVABLE | USABLE },
    };

    for (int i = 0; i < 5; ++i)
        add_item_to_room(items[i].r, create_item((char*)items[i].n, (char*)items[i].d, (unsigned int)items[i].flags));

    return w;
}

static struct room* __internal_register(struct container* w, const char* n, const char* d)
{
    struct room* r = create_room((char*)n, (char*)d);
    if (!r || !add_room_to_world(w, r))
    {
        if (r) free(r);
        return NULL;
    }
    return r;
}

struct container* add_room_to_world(struct container* w, struct room* r)
{
    if (!r)
        return NULL;

    struct container *cur = w;
    while (cur)
    {
        if (!strcasecmp(cur->room->name, r->name))
            return NULL;
        cur = cur->next;
    }

    return create_container(w, ROOM, r);
}

struct container* destroy_world(struct container* w)
{
    return destroy_containers(w);
}

struct room* get_room(struct container* w, char* n)
{
    return get_from_container_by_name(w, n);
}
