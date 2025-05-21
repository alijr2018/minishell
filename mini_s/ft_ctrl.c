/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctrl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:42:33 by abrami            #+#    #+#             */
/*   Updated: 2025/05/21 17:40:44 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_var;

/**this function for listing to Ctrl-C you have to add Ctrl-\ **/

void	lis(int i)
{
    if (i == SIGQUIT)
    {
        return ;
        // if i have to add costum thig to Ctrl-\ i have either use g_var for pid or finde other way
        //or to detect the command
    }
    if (g_var == 1)
	{
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void ft_sigaction()
{
    struct sigaction	sa;
    struct sigaction    ignore;
    
    //  SIGINT (Ctrl-C)
    sa.sa_flags = SA_RESTART;
	sa.sa_sigaction = lis;
	sigemptyset (&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
    }
    // Ignore SIGQUIT (Ctrl-\)
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if (sigaction(SIGQUIT, &ignore, NULL) == -1)
    {
        perror("sigaction SIGQUIT");
    }
}
