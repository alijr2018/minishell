/* ************************************************************************** */
/*	                                                                          */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:51:41 by abrami            #+#    #+#             */
/*   Updated: 2025/03/25 11:15:40 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern  int g_var;

//check with leaks
static void	ft_exit(char *input)
{
	// (void) input;
	printf("exit\n");
	if (input)
		free(input);
	rl_clear_history();
	exit (0);
}

static void ft_init(int ac, char **av, char **env, t_command *cmd)
{
	(void)ac;
	(void)av;
	char	*tmp;
	char	path[PATH_MAX];
	int		i;
	t_list	*list;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->heredoc_delimiter = NULL;
	cmd->append_output = 0;

	i = 0;
	if (!(*env))
	{
		tmp = ft_strdup("OLDPWD");
		if (!tmp || !add_to_list(&(cmd->env), tmp) || getcwd(path, PATH_MAX) == NULL)
			return ; // add here free
		tmp = ft_strjoin("PWD=", path);
		if (!tmp || !add_to_list(&(cmd->env), tmp))
			return ; // add here free
	}
	i = 0;
	list = NULL;
	while (env[i])
	{
		tmp = ft_strdup(env[i]);
		if (!tmp)
			return ; // add free
		if (!add_to_list(&list, tmp))
			return ; // add free
		i++;
	}
	cmd->env = list;
}
int	main(int ac, char **av, char **env)
{
	t_command	cmd;
	char		*input;

	ft_init(ac, av, env, &cmd);
	ft_sigaction();
	while (1)
	{
		g_var = 1;
		input = readline("minishell!>> ");
		g_var = -1;
		if (!input || ft_strcmp(input, "exit") == 0)
			ft_exit(input);
		if (*input)
			add_history(input);
		if (!parse(&cmd, input))
			continue;
		// if (!ft_executing(&data))
			// return ; // change it with free
		// printf("this is ---> %s :\n", input);
		// ft_executing(&input, env);
		// ft_execute(&input, env);
	}
	rl_clear_history();
	return (0);
}
