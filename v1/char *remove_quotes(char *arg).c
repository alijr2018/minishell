char *remove_quotes(char *arg)
{
	char	*result;
	int		i, j;
	char	q;

	if (!arg)
		return (NULL);
	result = malloc(ft_strlen(arg) + 1);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
		{
			q = arg[i++];
			while (arg[i] && arg[i] != q)
				result[j++] = arg[i++];
			if (arg[i] == q)
				i++; // skip closing quote
			else
			{
				// ❌ unclosed quote detected
				free(result);
				return (NULL);
			}
		}
		else
			result[j++] = arg[i++];
	}
	result[j] = '\0';
	return (result);
}

void print_arguments(char **args, int i, int interpret_backslashes)
{
	char	*arg;
	char	*processed_arg;

	while (args[i])
	{
		arg = remove_quotes(args[i]);
		if (!arg)
		{
			ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
			return;
		}
		processed_arg = process_escape_sequences(arg, &interpret_backslashes);
		printf("%s", processed_arg);
		if (processed_arg != arg)
			free(processed_arg);
		if (args[i + 1])
			printf(" ");
		free(arg); // because remove_quotes returns malloc'd memory
		i++;
	}
}


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int	builtin_echo(char **args)
{
	int		i = 1;
	bool	newline = true;

	// Check for -n option(s)
	while (args[i] && strncmp(args[i], "-n", 2) == 0 && strspn(args[i] + 2, "n") == strlen(args[i] + 2))
	{
		newline = false;
		i++;
	}

	// Print the rest of the arguments
	while (args[i])
	{
		write(1, args[i], strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}

	// Add newline if needed
	if (newline)
		write(1, "\n", 1);

	return (0);
}
