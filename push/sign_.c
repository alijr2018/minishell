#include "minishell.h"

void free_array(char **av){
    int i = 0;
    if (!av)
        return ;
    while (av[i])
    {
        free(av[i]);
        i++;
    }
    free(av);
}


void sigint_handl(int sig) {
    (void)sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int sig) {
    (void)sig;  // تجاهل المعامل لأننا لا نحتاجه
}