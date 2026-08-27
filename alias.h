#ifndef ALIAS_H
#define ALIAS_H

#define MAX_ALIAS 20

void set_alias(char *name, char *command);
char* get_alias(char *name);
void list_aliases();
void remove_alias(char *name);
void clear_aliases();

#endif
