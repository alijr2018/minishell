#include "minishell.h"

int main()
{
    char *input;

    while (1)
    {
        input = readline("mini$ ");
        if (!input)
        {
            ft_printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);
    }
    free(input);
    return (0);
}