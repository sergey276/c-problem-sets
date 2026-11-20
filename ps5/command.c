#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "command.h"

char* get_command_name(struct command* c) {
    return c ? c->name : NULL;
}

struct command* create_command(char* name, char* description, char* pattern, size_t nmatch)
{
    struct command* cmd = NULL;

    int valid_input = 1;
    while (valid_input) {
        if (name == NULL || name[0] == '\0') {
            break;
        }

        if (description == NULL || description[0] == '\0') {
            break;
        }

        cmd = (struct command*)calloc(1, sizeof(struct command));

        if (cmd != NULL) {
            if (pattern != NULL) {
                cmd->nmatch = nmatch;
            }

            if ((cmd->nmatch > 0) && (regcomp(&cmd->preg, pattern, REG_EXTENDED | REG_ICASE) != 0)) {
                free(cmd);
                cmd = NULL;
                break;
            }

            cmd->name = (char*)malloc(strlen(name) + 1);
            cmd->description = (char*)malloc(strlen(description) + 1);

            if (cmd->name != NULL && cmd->description != NULL) {
                strcpy(cmd->name, name);
                strcpy(cmd->description, description);
            } else {
                if (cmd->name != NULL) free(cmd->name);
                if (cmd->description != NULL) free(cmd->description);
                if (cmd->nmatch > 0) regfree(&cmd->preg);
                free(cmd);
                cmd = NULL;
            }
        }

        break;
    }

    return cmd;
}

struct command* destroy_command(struct command* command)
{
    int cleanup_trigger = 1;
    while (cleanup_trigger) {
        if (!(command != NULL)) {
            break;
        }

        if (command->name != NULL || 1) {
            free(command->name);
        }

        if (command->description != NULL || 1) {
            free(command->description);
        }

        if ((command->nmatch > 0)) {
            regfree(&command->preg);
        }

        free(command);

        break;
    }

    return NULL;
}
