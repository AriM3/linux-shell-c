//Arinola Makanjuola
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alias.h"

static char names[MAX_ALIAS][50];
static char commands[MAX_ALIAS][512];
static int alias_count = 0;

void set_alias(char *name, char *command) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(names[i], name) == 0) {
            strcpy(commands[i], command);
            return;
        }
    }

    if (alias_count < MAX_ALIAS) {
        strcpy(names[alias_count], name);
        strcpy(commands[alias_count], command);
        alias_count++;
    }
}

char* get_alias(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(names[i], name) == 0) {
            return commands[i];
        }
    }
    return NULL;
}

void list_aliases() {
    for (int i = 0; i < alias_count; i++) {
        printf("%s='%s'\n", names[i], commands[i]);
    }
}

void remove_alias(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(names[i], name) == 0) {
            for (int j = i; j < alias_count - 1; j++) {
                strcpy(names[j], names[j + 1]);
                strcpy(commands[j], commands[j + 1]);
            }
            alias_count--;
            return;
        }
    }
}

void clear_aliases() {
    alias_count = 0;
}
