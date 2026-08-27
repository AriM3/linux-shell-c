//gloria ukkatah
//group 8

//this file is used for singal handling for shell & child processes

#include <signal.h>

#include "signals.h"


void handle_sigint(int sig) {

    (void)sig;

}

void handle_sigtstp(int sig) {

    (void)sig;
}

void setup_shell_signals(void) {

    signal(SIGINT, handle_sigint);

    signal(SIGTSTP, handle_sigtstp);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    }

    void setup_child_signals(void) {

        signal(SIGINT, SIG_DFL);

        signal(SIGTSTP, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
