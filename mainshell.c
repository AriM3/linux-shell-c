//Fikayo Aiyegoro
//group 8

//this is the main shell, it is handles user input, cd, path, exit, pipes, processes & redirection

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "path.h"
#include "signals.h"
#include "redirection.h"
#include "exit.h"
#include "alias.h"


#define MAX_LINE 512
#define MAX_ARGS 100


void parse_command(char *cmd, char *args[]) { //cmd string split into 2 arguments

    int i =0;

    char *token = strtok(cmd, " ");

    while (token != NULL && i < MAX_ARGS -1) {

        args[i++] = token;

        token = strtok(NULL, " ");
    }

    args[i] = NULL;
}



int main(int argc, char *argv[]) { // main shell loop

    load_initial_path();

    setup_shell_signals();

    setpgid(0,0);

    tcsetpgrp(STDIN_FILENO, getpgrp());

    char line[MAX_LINE];

    while(1) {

        printf("mainshell> "); // shows the shell prompt

        fflush(stdout);

        if(fgets(line, sizeof(line), stdin) == NULL) { // read the input from user

            printf("\n");

            break;
        }

        line[strcspn(line, "\n")] = '\0';
      

        if (strlen(line) == 0) {

            continue;
        }

        if (strchr(line, '<') != NULL || strchr(line, '>')  != NULL) { //input & output redirection

            execute_with_redirection(line);

            continue;
        }

        int pipe_count = 0;

        for (int j = 0; line[j] != '\0'; j++) {


            if (line[j] == '|') {

                pipe_count++;
            }
        }


        if (pipe_count == 2) {

            char *cmd1 = strtok(line, "|");

            char *cmd2 = strtok(NULL, "|");

            char *cmd3 = strtok(NULL, "|");


            if (cmd1 ==NULL || cmd2 == NULL || cmd3 == NULL) {

                fprintf(stderr, "Invalid pipe\n");

                continue;
            }


            char *args1[MAX_ARGS];
            char *args2[MAX_ARGS];
            char *args3[MAX_ARGS];


            parse_command(cmd1, args1);
            parse_command(cmd2, args2);
            
            parse_command(cmd3, args3);


            int fd1[2], fd2[2];


            if (pipe(fd1) == -1 || pipe(fd2) == -1) {

                perror("pipe failed");

                continue;
            }


            pid_t pid1 = fork();

            if(pid1 < 0 ) {


                perror ("fork failed");

                close(fd1[0]); close(fd1[1]);

                close(fd2[0]); close(fd2[1]);

                continue;
            }

            if (pid1 == 0) {


                dup2(fd1[1], STDOUT_FILENO);
                close(fd1[0]); close(fd1[1]);

                close(fd2[0]); close(fd2[1]);

                setpgid(0,0);
                setup_child_signals();

                execvp(args1[0], args1);

                perror("exec failed");

                exit(1);
            }

            pid_t pid2 = fork();

            if (pid2 < 0) {


                perror("fork failed");

                close(fd1[0]); close(fd1[1]);
                close(fd2[0]); close(fd2[1]);

                waitpid(pid1, NULL, 0);


                continue;
            }

            if (pid2 == 0) {

                dup2(fd1[0], STDIN_FILENO);
                dup2(fd2[1], STDOUT_FILENO);

                close(fd1[0]); close(fd1[1]);
                close(fd2[0]); close(fd2[1]);

                setpgid(0,0);
                setup_child_signals();

                execvp(args2[0], args2);

                perror("exec failed");

                exit(1);
            }

            pid_t pid3 = fork();

            if (pid3 < 0) {

                perror("fork failed");

                close(fd1[0]); close(fd1[1]);

                close(fd2[0]); close(fd2[1]);




                waitpid(pid1, NULL, 0);

                waitpid(pid2, NULL, 0);

                continue;
            }


            if(pid3 == 0) {

                dup2(fd2[0], STDIN_FILENO);

                close(fd1[0]); close(fd1[1]);

                close(fd2[0]); close(fd2[1]);

                setpgid(0,0);
                setup_child_signals();

                execvp(args3[0], args3);

                perror("exec failed");


                exit(1);
            }

            close(fd1[0]); close(fd1[1]);

            close(fd2[0]); close(fd2[1]);

            setpgid(pid1, pid1);
            setpgid(pid2, pid1);
            setpgid(pid3, pid1);

            tcsetpgrp(STDIN_FILENO, pid1);

            waitpid(pid1, NULL, 0);

            waitpid(pid2, NULL, 0);

            waitpid(pid3, NULL, 0);

            tcsetpgrp(STDIN_FILENO, getpgrp());
            continue;

        }


        else if (strchr(line, '|')) { // handles the pipe cmds

            char *left = strtok(line, "|");

            char *right = strtok(NULL, "|");


            if(left == NULL || right == NULL) {

                fprintf( stderr,  "Invalid pipe\n");

                continue;
            }
                char *args1[MAX_ARGS];

                char *args2[MAX_ARGS];


                parse_command(left, args1);

                parse_command(right, args2);


                int fd[2];


                if(pipe(fd) == -1) {


                    perror("pipe failed");


                    continue;
                }

                pid_t pid1 = fork();

                if(pid1 < 0) {

                    perror("fork failed");

                    close(fd[0]);

                    close(fd[1]);


                    continue;
                }


                if(pid1 == 0) {

                    setpgid(0,0);

                    setup_child_signals();

                    dup2(fd[1],  STDOUT_FILENO);

                    close(fd[0]);

                    close(fd[1]);


                    execvp(args1[0], args1);


                    perror("exec failed");

                    exit(1);
                }


                pid_t pid2 = fork();


                if(pid2 < 0 ) {

                    perror ("fork failed");

                    close(fd[0]);

                    close(fd[1]);


                    waitpid(pid1, NULL, 0);


                    continue;
                }


                if(pid2 == 0) {

                    setpgid(0,0);

                    setup_child_signals();

                    dup2(fd[0], STDIN_FILENO);

                    close(fd[1]);

                    close(fd[0]);


                    execvp(args2[0], args2);


                    perror("exec failed");

                    exit(1);
                }


                close(fd[0]);

                close(fd[1]);


                setpgid(pid1, pid1);

                setpgid(pid2, pid1);

                tcsetpgrp(STDIN_FILENO, pid1);



                waitpid(pid1, NULL, 0);

                waitpid(pid2, NULL, 0);

                tcsetpgrp(STDIN_FILENO, getpgrp());

                continue;
            }




        if(is_exit_command(line)) { 

           exit(0); 
        }
        
        char *args[MAX_ARGS];

        int i = 0;

        char *token = strtok(line, " ");

        while (token != NULL && i < MAX_ARGS -1) {


            args[i++] = token;


            token = strtok(NULL, " ");


        }

        args[i] = NULL;

        

        if(strcmp(args[0], "path") == 0) {

            if(args[1] == NULL) {

                print_path();
            }

            else if (args[2] == NULL || args[3] != NULL ) {

                fprintf(stderr, "path: invalid usage\n");
            }

            else if (strcmp(args[1], "+") == 0) {

                add_path(args[2]);
            }

            else if (strcmp(args[1], "-") ==0) {

                remove_path(args[2]);
            }

            else {

                fprintf(stderr, "path: invalid usage\n");
            }

            continue;
        }

        if (strcmp(args[0], "cd") ==0) {

            if(args[1] == NULL) {

                char *home = getenv ("HOME");

                if (home == NULL) {

                    fprintf(stderr, "cd: HOME not srt\n");
                }


                else if(chdir(home) != 0) {

                    perror("cd failed");
                }
            }

            else {
                if (chdir(args[1]) != 0) {

                    perror ("cd failed");
                }
        }
        
        continue;
        }

        pid_t pid = fork();  //create the child process 


        if (pid < 0) {



        perror("fork failed");

        continue;

        } else if (pid ==0) {

            setpgid(0,0);

            setup_child_signals();

            execvp(args[0], args);


            perror("exec failed");

            exit(1);

        } else {

            setpgid(pid, pid);

            tcsetpgrp(STDIN_FILENO, pid);

            waitpid(pid, NULL, 0);

            tcsetpgrp(STDIN_FILENO, getpgrp());
        }
    }


            setenv("PATH", original_path, 1);
    return 0;
        }
    
