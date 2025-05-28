/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:15:30 by abrami            #+#    #+#             */
/*   Updated: 2025/05/27 17:03:02 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//i need to create a function to parse the input
int has_unclosed_quotes(t_command *cmd, const char *str)
{
    int	single_quote;
    int	double_quote;

    single_quote = 0;
    double_quote = 0;
    while (*str)
    {
        if (*str == '\'' && double_quote == 0)
            single_quote = !single_quote;
        else if (*str == '"' && single_quote == 0)
        double_quote = !double_quote;
        str++;
    }
    if (single_quote || double_quote)
    {
        perror("quotes open");
        cmd->code_exit = 2;
        return (1);
    }
    return (0);
}

bool	list_cmd(t_command *cmd)
{
    t_token *tmp;
    
    tmp = cmd->token;

}
bool    parse(t_command *cmd, char  *input)
{
    if (has_unclosed_quotes(cmd, input))
        return (false); // add free
    if (cmd->token && cmd->token->prev->type == PIPE)
    {
        perror("pipe open");
        cmd->code_exit = 2;
        return (false);
    }
    if (!cmd->token || !list_cmd(cmd))
        return (false);
}

