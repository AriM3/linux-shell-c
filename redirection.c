//Student: Michael Idwou
//Group: 8
//Program: Extra feature  redirection
//Program Description: This program extends the shell with I/O redirection


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "signals.h"
#include "redirection.h"

//command is split in arguments
char **parse_args(char *cmd)
{
    int size = 10;
    int count = 0;
    char **args = malloc(sizeof(char *) * size);

    if (args == NULL)
    {
        perror("malloc");
        exit(1);
    }

    char *token = strtok(cmd, " \t\n");

    while (token != NULL)
    {
        args[count++] = token;

        if (count >= size)
        {
            size *= 2;
            char **temp = realloc(args, sizeof(char *) * size);
            if (temp == NULL)
            {
                free(args);
                perror("realloc");
                exit(1);
            }
            args = temp;
        }

        token = strtok(NULL, " \t\n");
    }

    args[count] = NULL;
    return args;
}

//parse redirection
int parse_redirection(char **args, char **file)
{
    int i = 0;
    int type = 0;

    *file = NULL;

    while (args[i] != NULL)
    {
        if (strcmp(args[i], "<") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Error: missing input file\n");
                return -1;
            }
            type = 1;
            *file = args[i + 1];
            args[i] = NULL;
            break;
        }

        if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Error: missing output file\n");
                return -1;
            }
            type = 2;
            *file = args[i + 1];
            args[i] = NULL;
            break;
        }

        i++;
    }

    return type;
}

//commands are executed with redirection
void execute_with_redirection(char *command)
{
    char *cmd_copy = strdup(command);
    char **args;
    char *file = NULL;
    int type;
    pid_t pid;

    if (cmd_copy == NULL)
    {
        perror("strdup");
        return;
    }

    args = parse_args(cmd_copy);

    if (args[0] == NULL)
    {
        free(cmd_copy);
        free(args);
        return;
    }

    type = parse_redirection(args, &file);

    if (type == -1)
    {
        free(cmd_copy);
        free(args);
        return;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(cmd_copy);
        free(args);
        return;
    }

    if (pid == 0)
    {

        setpgid(0,0);
        setup_child_signals();

        if (type == 1)
        {
            int fd = open(file, O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }

            close(fd);
        }

        if (type == 2)
        {
            int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }

            close(fd);
        }

        execvp(args[0], args);
        fprintf(stderr, "Command not found\n");
        exit(1);
    }
    else
    {
        wait(NULL);
    }

    free(cmd_copy);
    free(args);
}

