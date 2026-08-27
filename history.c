//Arinola Makanjuola
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"

static char history[HISTORY_SIZE][512];
static int count = 0;

void add_history(char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0) return;

    strncpy(history[count % HISTORY_SIZE], cmd, 511);
    history[count % HISTORY_SIZE][511] = '\0';
    count++;
}

void show_history() {
    int start = (count > HISTORY_SIZE) ? count - HISTORY_SIZE : 0;

    for (int i = start; i < count; i++) {
        printf("%d %s\n", i - start + 1, history[i % HISTORY_SIZE]);
    }
}

void clear_history() {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i][0] = '\0';
    }
    count = 0;
}

void execute_history(int index, char **args) {
    int start = (count > HISTORY_SIZE) ? count - HISTORY_SIZE : 0;

    if (index < 1 || index > (count - start)) {
        fprintf(stderr, "Invalid history index\n");
        return;
    }

    char *get_history_command(int index) {

        int start = (count > HISTORY_SIZE) ? count - HISTORY_SIZE : 0;

        int total = count - start;

        if (index <1 || index > total) {

            fprintf(stderr, "Invalid history index\n");

            return NULL;
        }

        return history [(start + index -1) % HISTORY_SIZE];
    }

}
