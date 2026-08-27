#ifndef HISTORY_H
#define HISTORY_H

#define HISTORY_SIZE 20

void add_history(char *cmd);
void show_history(void);
void clear_history(void);
char *get_history_command(int index);

#endif
