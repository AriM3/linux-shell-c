//Gloria Ukkatah
//grup 8
// path management for shell functions

#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include <string.h>


char shell_paths[MAX_PATHS][MAX_PATH_LEN]; //this is used to store shell path directories

int path_count =0;

char original_path[4096];

void load_initial_path(void) { // this is used to load the current path from environment

    char *env_path = getenv("PATH");



    if (env_path == NULL ) {

        original_path[0] = '\0';

        path_count =0;

        return;
    }



    strncpy(original_path, env_path, sizeof(original_path) -1);

    original_path[sizeof(original_path) -1] = '\0';


    char temp[4096];

    strncpy(temp, env_path, sizeof(temp) -1);


    temp[sizeof(temp) -1] = '\0';


    path_count = 0;

    char *token = strtok(temp, ":");



    while (token != NULL && path_count < MAX_PATHS) {

        strncpy(shell_paths[path_count], token, MAX_PATH_LEN -1);

        shell_paths[path_count][MAX_PATH_LEN -1] = '\0';

        path_count++;

        token = strtok(NULL, ":");
    }
}



void update_path_env(void) {  // this is used to update path environment variable

    char new_path[4096] = "";

    int i;

    for (i = 0; i < path_count; i++) {

        strcat(new_path, shell_paths[i]);

        if ( i < path_count - 1) {

            strcat(new_path, ":");
        }
    }

    setenv("PATH", new_path, 1);
}


void print_path(void) {  // this is used to print current path list

    int i;


    for (i = 0; i < path_count; i++) {

        printf("%s", shell_paths[i]);

        if(i < path_count -1) {

            printf(":");

        }
    }

    printf("\n");
}


void add_path(const char *newdir) { // this is used to add directory to path
    int i;

    if(path_count >= MAX_PATHS) {

        fprintf(stderr, "path: too many entires\n");


        return;
    }


    for (i = 0; i < path_count; i++) {

        if (strcmp(shell_paths[i], newdir) == 0) {

            return;
        }
    }


    strncpy(shell_paths[path_count], newdir, MAX_PATH_LEN -1);


    shell_paths[path_count][MAX_PATH_LEN -1] = '\0';

    path_count++;

    update_path_env();
}


void remove_path(const char *target) { // this is used to remove directory from path

    int i, j;

    for( i = 0; i < path_count; i++) {

        if(strcmp(shell_paths[i], target) == 0) {

            for (j = i; j < path_count -1; j++) {

                strcpy(shell_paths[j], shell_paths[j+1]);

            }
        
            path_count--;

            update_path_env();

            return;        
        }
    }
}
