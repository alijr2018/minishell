#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

// === START: Mock ft_strdup, ft_strchr, ft_split ===
char	**ft_splited(const char *s);

char *ft_strdup(const char *s) {
    char *dup = malloc(strlen(s) + 1);
    if (dup) strcpy(dup, s);
    return dup;
}

char *ft_strchr(const char *s, int c) {
    return strchr(s, c);
}

char **ft_split(const char *s, char delim) {
    int count = 1;
    for (const char *p = s; *p; p++) if (*p == delim) count++;
    char **result = malloc((count + 1) * sizeof(char *));
    const char *start = s;
    int i = 0;
    for (const char *p = s; ; p++) {
        if (*p == delim || *p == '\0') {
            int len = p - start;
            result[i] = malloc(len + 1);
            strncpy(result[i], start, len);
            result[i][len] = '\0';
            i++;
            if (*p == '\0') break;
            start = p + 1;
        }
    }
    result[i] = NULL;
    return result;
}

void ft_free_strarray(char **arr) {
    for (int i = 0; arr[i]; i++) free(arr[i]);
    free(arr);
}

// === END: Mock helpers ===

typedef struct s_command {
    char **args;
} t_command;

char *searchexec(char *str, char **env) {
    if (!str || !*str)
        return NULL;

    if (ft_strchr(str, '/')) {
        if (access(str, X_OK) == 0)
            return ft_strdup(str);
        return NULL;
    }

    char *path = NULL;
    for (int i = 0; env && env[i]; i++) {
        if (strncmp(env[i], "PATH=", 5) == 0) {
            path = env[i] + 5;
            break;
        }
    }

    if (!path)
        return NULL;

    char **paths = ft_split(path, ':');
    if (!paths)
        return NULL;

    for (int i = 0; paths[i]; i++) {
        char *full = malloc(strlen(paths[i]) + strlen(str) + 2);
        sprintf(full, "%s/%s", paths[i], str);
        if (access(full, X_OK) == 0) {
            char *res = ft_strdup(full);
            free(full);
            ft_free_strarray(paths);
            return res;
        }
        free(full);
    }

    ft_free_strarray(paths);
    return NULL;
}

void ft_execute(char *command, char **env) {
    if (!command) {
        fprintf(stderr, "Empty command\n");
        exit(1);
    }

    char *exec_path = searchexec(command, env);
    if (!exec_path) {
        fprintf(stderr, "Command not found: %s\n", command);
        exit(127);
    }

    printf("Executing: %s -> %s\n", command, exec_path);

    char *argv[] = {command, NULL};

    execve(exec_path, argv, env);
    perror("execve failed");
    free(exec_path);
    exit(1);
}

void ft_executing(t_command *cmd, char **env) {
    int i = 0;
    pid_t pid;

    if (!cmd || !cmd->args)
        return;

    while (cmd->args[i]) {
        printf("Forking for command: %s\n", cmd->args[i]);

        pid = fork();
        if (pid == -1) {
            perror("fork");
            i++;
            continue;
        }

        if (pid == 0) {
            ft_execute(cmd->args[i], env);
        }
        i++;
    }

    while (wait(NULL) > 0)
        ;
}
int parses(t_command *cmd, char *input)
{

	cmd->args = ft_splited(input);  // your split function
	if (!cmd->args)
    {
        // free_char_array(cmd->args);
		return 1;
    }
	return 0;
}
int main(int argc, char **argv, char **env) {
    t_command cmd;
	char *input;
    // char *commands[] = {"ls", "pwd", "whoami", NULL};
    // cmd.args = commands;
	while (1)
	{
		// g_var = 1;
		input = readline("minishell!>> ");
		// g_var = -1;
		if (!input)
		{
			// if (cmd.args != NULL)
			// 	free_char_array(cmd.args);
			// ft_exit(input);
			exit(0);
		}
		if (*input)
			add_history(input);
		// if (!parse(&cmd, input))
		// 	continue;
		if ((parses(&cmd, input)))
			continue;
			// free_char_array(cmd.args);
		ft_executing(&cmd, env);
		// if (!ft_executing(&data))
		// 	return ; // change it with free
		// printf("this is ---> %s :\n", input);
		// ft_executing(&input, env);
		// ft_execute(&input, env);
	}
	rl_clear_history();
    return 0;
}


// # include <unistd.h>
// # include <stdlib.h>
// # include <stdbool.h>
// # include <stdarg.h>
// # include <signal.h>
// # include <fcntl.h>
// # include <sys/wait.h>
// # include <stdio.h>
// # include <readline/readline.h>
// # include <readline/history.h>
// # include <limits.h>
// //this just for tests


// void	lis(int i)
// {
// 	(void)i;
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// int	main(void)
// {
// 	char	*input;

// 	signal(SIGINT, lis);
// 	while (1)
// 	{

// 		input = readline("minishell!>> ");

// 		if (!input)
//         {
//             printf("sad\n");
//         }
//         if (*input)
//             add_history(input);
//     }
//     rl_clear_history();
// 	return (0);

// }
