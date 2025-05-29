/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:15:30 by abrami            #+#    #+#             */
/*   Updated: 2025/05/28 17:27:43 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//i need to create a function to parse the input

int ft_strncmp(char *srt, char *str, int n)
{
	int	i;

	i = 0;
	while (str[i] && srt[i] && str[i] == srt[i] && i <= n)
		i++;
	return (str[i] - srt[i]);
}
int	ft_isalpha(int c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return (1);
	else
		return (0);
}

bool	print_error(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (true);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	if (arr)
		free(arr);
	arr = NULL;
}

static void	free_all_cmd(t_cmd *tmp)
{
	if (tmp->in_file > 0)
		close(tmp->in_file);
	tmp->in_file = -2;
	if (tmp->out_file > 0)
		close(tmp->out_file);
	tmp->out_file = -2;
	free_array(tmp->cmd_param);
}
void	free_cmd(t_cmd **list)
{
	t_cmd	*tmp;
	t_cmd	*current;

	if (!(*list))
		return ;
	current = *list;
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free_all_cmd(tmp);
		free(tmp);
	}
	free_all_cmd(current);
	free(current);
	*list = NULL;
}
int	free_list(t_list **list)
{
	t_list	*tmp;
	t_list	*current;

	if (!(*list))
		return (0);
	current = *list;
	while (current->next_one != *list)
	{
		tmp = current;
		current = current->next_one;
		free(tmp->str);
		free(tmp);
	}
	free(current->str);
	free(current);
	*list = NULL;
	return (0);
}
void	free_token(t_token **list)
{
	t_token	*tmp;
	t_token	*current;

	if (!(*list))
		return ;
	current = *list;
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free(tmp->str);
		free(tmp);
	}
	free(current->str);
	free(current);
	*list = NULL;
}
void	free_all(t_command *cmd, char *err, int ext)
{
	if (cmd->commands)
		free_cmd(&cmd->commands);
	if (cmd->token)
		free_token(&cmd->token);
	if (cmd->env)
		free_list(&cmd->env);
	if (cmd->pip[0] && cmd->pip[0] != -1)
		close(cmd->pip[0]);
	if (cmd->pip[1] && cmd->pip[1] != -1)
		close(cmd->pip[1]);
	if (err)
		print_error(err);
	rl_clear_history();
	if (!access(".heredoc.tmp", F_OK))
		unlink(".heredoc.tmp");
	if (ext != -1)
		exit(ext);
}
int     ft_isalnum(int c)
{
        if (((c >= 97 && c <= 122) || (c >= 65 && c <= 90)) || (c >= 48 && c <= 57))
                return (1);
        else
                return (0);
}
void	quoting_choice(bool *dq, bool *sq, int *index, char c)
{
	if ((c == '\'' || c == '"') && !*sq && !*dq)
	{
		if (c == '\'' && !*dq)
			*sq = true;
		else if (c == '"' && !*sq)
			*dq = true;
		if (index)
			++(*index);
	}
	else if ((c == '\'' || c == '"'))
	{
		if (c == '\'' && !*dq && *sq)
			*sq = false;
		else if (c == '"' && !*sq && *dq)
			*dq = false;
		if (index)
			++(*index);
	}
}
size_t	len_list(t_list *list)
{
	t_list	*tmp;
	size_t	i;

	if ((list))
	{
		tmp = list;
		i = 1;
		while (tmp->next_one != list)
		{
			++i;
			tmp = tmp->next_one;
		}
		return (i);
	}
	return (0);
}
static int	ft_search(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (0);
}

static int	end_word(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	if (i == ft_search(env, '='))
		return (i);
	return (0);
}

/* return 1 si $VAR dans env sinon 0 */
int	exist_in_env(char *line, int *i, t_command *cmd)
{
	t_list	*tmp;
	int		len;

	if (line[*i + 1] == '?' || line[*i + 1] == '$')
		return (2);
	tmp = cmd->env;
	len = len_list(tmp);
	while (len--)
	{
		if (ft_strncmp(tmp->str, &line[*i + 1], \
			end_word(&line[*i + 1], tmp->str)) == 0)
		{
			*i += ft_strlen(tmp->str) - \
				ft_strlen(ft_strchr(tmp->str, '=')) + 1;
			return (1);
		}
		tmp = tmp->next_one;
	}
	return (0);
}
int	add_char(char *c, char **str, t_command *cmd, int *index)
{
	char	char_to_str[2];
	char	*tmp2;
	int		i;

	i = 0;
	if (c[i] == '$' && !cmd->sq && exist_in_env(c, &i, cmd))
		return (1);
	char_to_str[0] = *c;
	char_to_str[1] = '\0';
	(*index)++;
	tmp2 = ft_strjoin(*str, char_to_str);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}
int	replace_dollar(char **line, t_command *cmd)
{
	bool	dq;
	int		i;
	char	*str;

	i = 0;
	dq = false;
	cmd->sq = false;
	str = ft_strdup("");
	while ((*line)[i])
	{
		quoting_choice(&dq, &cmd->sq, NULL, (*line)[i]);
		if ((*line)[i] && (*line)[i + 1] && (*line)[i] == '$' && \
			((*line)[i + 1] != '\'' && (*line)[i + 1] != '"') && \
			(ft_isalpha((*line)[i + 1]) || (*line)[i + 1] == '?' || \
			(*line)[i + 1] == '_') && !cmd->sq)
			return (0);
		if ((*line)[i] && !add_char(&(*line)[i], &str, cmd, &i))
			return (0);
	}
	free(*line);
	*line = str;
	return (1);
}

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

static bool	read_in_stdin(t_command *cmd, int fd, char *word)
{
	char	*buf;

	while (1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			print_error("warning: here-document delimited by end-of-file ");
			print_error("(wanted '");
			print_error(word);
			print_error("')\n");
			break ;
		}
		if (!ft_strncmp(word, buf, INT_MAX))
			break ;
		if (!replace_dollar(&buf, cmd))
			free_all(cmd, ERR_MALLOC, EXT_MALLOC);
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
	}
	free(buf);
	close(fd);
	return (true);
}
static int	here_doc(t_command *cmd, char *word)
{
	int	fd;

	fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (!read_in_stdin(cmd, fd, word))
	{
		unlink(".heredoc.tmp");
		return (-1);
	}
	fd = open(".heredoc.tmp", O_RDONLY);
	if (fd > 0)
		unlink(".heredoc.tmp");
	return (fd);
}
static int	open_file(t_command *cmd, char *filename, int type)
{
	int	fd;

	fd = -2;
	if (type == INPUT)
		fd = open(filename, O_RDONLY, 0644);
	else if (type == HEREDOC)
		fd = here_doc(cmd, filename);
	else if (type == TRUNC)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (type != HEREDOC && fd < 0)
		perror(filename);
	return (fd);
}
static bool	print_error_token(t_token *token, t_command *cmd)
{
	write(2, "syntax error near unexpected token ", 35);
	write(2, "'", 1);
	if (token->next == cmd->token)
		write(2, "newline", 7);
	else
		write(2, token->next->str, ft_strlen(token->next->str));
	write(2, "'\n", 2);
	return (false);
}
static bool	get_in(t_command *cmd, t_token *tmp, t_cmd *commands)
{
	if (tmp->type == INPUT)
	{
		if (commands->in_file >= 0)
			close(commands->in_file);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, cmd));
		commands->in_file = open_file(cmd, tmp->next->str, INPUT);
		if (commands->in_file == -1)
			return (false);
	}
	else if (tmp->type == HEREDOC)
	{
		if (commands->in_file >= 0)
			close(commands->in_file);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, cmd));
		commands->in_file = open_file(cmd, tmp->next->str, HEREDOC);
		if (commands->in_file == -1)
			return (false);
	}
	return (true);
}

bool    input_file(t_command *cmd, t_token *token, t_cmd *commands)
{
    t_token	*tmp;

	tmp = token;
	if (tmp->type != PIPE && !get_in(cmd, tmp, commands))
		return (false);
	if (tmp->type == PIPE)
		return (true);
	tmp = tmp->next;
	while (tmp->type != PIPE && tmp != cmd->token)
	{
		if (!get_in(cmd, tmp, commands))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static bool	get_out(t_token *tmp, t_cmd *commands, t_command *cmd)
{
	if (tmp->type == TRUNC)
	{
		if (commands->out_file >= 0)
			close(commands->out_file);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, cmd));
		commands->out_file = open_file(NULL, tmp->next->str, TRUNC);
		if (commands->out_file == -1)
			return (false);
	}
	else if (tmp->type == APPEND)
	{
		if (commands->out_file >= 0)
			close(commands->out_file);
		if (tmp == tmp->next || tmp->next->type <= 5)
			return (print_error_token(tmp, cmd));
		commands->out_file = open_file(NULL, tmp->next->str, APPEND);
		if (commands->out_file == -1)
			return (false);
	}
	return (true);
}

bool	ft_out_file(t_token *token, t_cmd *commands, t_command *cmd)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->type != PIPE && !get_out(tmp, commands, cmd))
		return (false);
	tmp = tmp->next;
	while (tmp != cmd->token && tmp->type != PIPE)
	{
		if (!get_out(tmp, commands, cmd))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}
static  bool    put_cmd(t_command *cmd, t_token *tmp)
{
    if (!input_file(cmd, tmp, cmd->commands->previous) && \
        cmd->commands->previous->in_file != -1)
        return (false);
    if (cmd->commands->previous->in_file == -1)
    {
        cmd->commands->previous->skip_cmd = true;
        cmd->commands->previous->out_file = -1;
        return (true);
    }
    if (!ft_out_file(tmp, cmd->commands->previous, cmd) && \
        cmd->commands->previous->out_file != -1)
        return (false);
    if (cmd->commands->previous->out_file == -1)
    {
		if (cmd->commands->previous->in_file >= 0)
			close (cmd->commands->previous->in_file);
		cmd->commands->previous->skip_cmd = true;
		cmd->commands->previous->in_file = -1;
		return (true);
	}
	// cmd->commands->previous->cmd_param = get_param(cmd, tmp);
	if (!cmd->commands->previous->cmd_param)
		free_all(cmd, ERR_MALLOC, EXT_MALLOC);
	return (true);
}
static  bool    norm(t_command *cmd, t_token *tmp)
{
    if (!add_to_list_cmd(&cmd->commands, -2, -2, NULL))
        free_all(cmd, ERR_MALLOC, EXT_MALLOC);
    if (!put_cmd(cmd, tmp))
    {
        cmd->code_exit = 2;
        return (false);
    }
    return (true);
}
bool	list_cmd(t_command *cmd)
{
    t_token *tmp;
    
    tmp = cmd->token;
	if (!tmp)
		return false;
    tmp = tmp->next;
    while (tmp != cmd->token)
    {
        if (tmp->previous->type == PIPE)
        {
            if (!norm(cmd, tmp))
                return (false);
        }
        tmp = tmp->next;
    }    
    return (true);
}
bool	check_pipe(t_command *cmd)
{
	if (cmd->token->type == PIPE)
	{
		write(2, "syntax error near unexpected token '|'\n", 39);
		return (false);
	}
	return (true);
}

bool    parse(t_command *cmd, char  *input)
{
    if (has_unclosed_quotes(cmd, input))
        return (false); // add free
    if (cmd->token && cmd->token->previous->type == PIPE)
    {
        perror("pipe open");
        cmd->code_exit = 2;
        return (false);
    }
    if (!cmd->token || !list_cmd(cmd))
	{
        return (false);
		
	}
	return (check_pipe(cmd));
}

