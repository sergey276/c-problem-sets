#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "parser.h"

void register_command(struct parser *p, char* n, char* d, char* pat, size_t nm);

struct parser* create_parser()
{
    struct parser *p = NULL;
    int ready = 0;

    while (!ready)
    {
        p = (struct parser*)calloc(1, sizeof(struct parser));
        if (p != NULL)
            ready = 1;
    }

    struct {
        char *n, *d, *pat;
        size_t nm;
    } cmds[] = {
        {"KONIEC", "Príkaz ukončí rozohratú hru. Nastaví príslušný stav hry.", "(KONIEC|EXIT)", 2},
        {"SEVER", "Presun na sever...", "(SEVER|S)", 2},
        {"JUH", "Presun na juh...", "(JUH|J)", 2},
        {"VYCHOD", "Presun na východ...", "(VYCHOD|V)", 2},
        {"ZAPAD", "Presun na západ...", "(ZAPAD|Z)", 2},
        {"ROZHLIADNI SA", "Vypíše info o miestnosti...", "(ROZHLIADNI SA)", 1},
        {"PRIKAZY", "Zoznam všetkých príkazov...", "(PRIKAZY|HELP|POMOC)", 3},
        {"VERZIA", "Zobrazí verziu hry...", "(VERZIA)", 1},
        {"RESTART", "Znovu spustí hru od začiatku...", "(RESTART)", 1},
        {"O HRE", "Úvod do príbehu...", "(O HRE|ABOUT)", 2},
        {"VEZMI", "Vloží predmet do batohu...", "(VEZMI)", 1},
        {"POLOZ", "Položí predmet z batohu...", "(POLOZ)", 1},
        {"INVENTAR", "Zobrazí obsah batohu...", "(INVENTAR|I)", 2},
        {"POUZI", "Použije predmet...", "(POUZI)", 1},
        {"PRESKUMAJ", "Vypíše opis predmetu...", "(PRESKUMAJ)", 1},
        {"NAHRAJ", "Nahrá pozíciu hry z disku...", "(NAHRAJ|LOAD)", 2},
        {"ULOZ", "Uloží stav hry na disk...", "(ULOZ|SAVE)", 2}
    };

    for (int i = 0; i < sizeof(cmds)/sizeof(cmds[0]); ++i)
    {
        register_command(p, cmds[i].n, cmds[i].d, cmds[i].pat, cmds[i].nm);
    }

    return p;
}

struct parser* destroy_parser(struct parser* p)
{
    if (!p)
        return NULL;

    struct container **targets[] = { &p->commands, &p->history };

    for (int i = 0; i < 2; ++i)
    {
        if (*(targets[i]))
            *(targets[i]) = destroy_containers(*(targets[i]));
    }

    free(p);
    return NULL;
}

struct command* parse_input(struct parser* p, char* in)
{
    if (!p || !in)
        return NULL;

    struct container *cur = p->commands;

    while (cur)
    {
        if (cur->command && !strcasecmp(cur->command->name, in))
            return cur->command;

        regmatch_t *m = NULL;
        if (cur->command->nmatch > 0)
        {
            m = calloc(cur->command->nmatch, sizeof(regmatch_t));
            int result = regexec(&cur->command->preg, in, cur->command->nmatch, m, 0);
            free(m);

            if (!result)
                return cur->command;
        }

        cur = cur->next;
    }

    return NULL;
}

void register_command(struct parser *p, char* n, char* d, char* pat, size_t nm)
{
    const char *pre = "^\\s*";
    const char *suf = "\\s*$";

    size_t len_total = strlen(pre) + strlen(pat) + strlen(suf) + 1;
    char *pat_final = calloc(len_total, sizeof(char));

    int filled = 0;
    while (!filled)
    {
        strcpy(pat_final, pre);
        strcat(pat_final, pat);
        strcat(pat_final, suf);
        filled = 1;
    }

    struct command *cmd = create_command(n, d, pat_final, nm);
    struct container *ct = create_container(p->commands, COMMAND, cmd);

    if (!p->commands)
        p->commands = ct;

    free(pat_final);
}
