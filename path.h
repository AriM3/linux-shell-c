//gloria ukkatah
//group 8
//this file is the header for path functions

#ifndef PATH_H

#define MAX_PATHS 100

#define MAX_PATH_LEN 1024

extern char shell_paths[MAX_PATHS][MAX_PATH_LEN];

extern int path_count;

extern char original_path[4096];

void load_initial_path(void);

void update_path_env(void);

void print_path(void);

void add_path(const char *newdir);

void remove_path(const char *target);


#endif
