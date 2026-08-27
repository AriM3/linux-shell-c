//Student: Michael Idwou
//Group: 8
//Program: Built-in Exit
//Program Description: This program exits the shell itself using the exit() system call


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//check if the command is the exit command
int is_exit_command(char *cmd)
{
    if (cmd == NULL) return 0;

    while (*cmd == ' ' || *cmd == '\t') cmd++;

    size_t len = strlen(cmd);
    while (len > 0 && (cmd[len - 1] == '\n' || cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
    {
        cmd[len - 1] = '\0';
        len--;
    }

    return strcmp(cmd, "exit") == 0;
}


/*
//seperates the process commands using ;
void process_commands(char *line)
{
    char *command;
    char *saveptr;
    int should_exit = 0;

    command = strtok_r(line, ";", &saveptr);

    while (command != NULL)
    {
        while (*command == ' ' || *command == '\t') command++;

        if (*command != '\0')
        {
            if (is_exit_command(command))
            {
                should_exit = 1;
            }
            else
            {
                printf("Executing: %s\n", command);
            }
    }

        command = strtok_r(NULL, ";", &saveptr);
    }

    //exits after all commands finish
    if (should_exit)
    {
        exit(0);
    }
}
*/

