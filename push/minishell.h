#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"

#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <limits.h>

#include <signal.h> // her
#include <errno.h> //""""""""""""""""""""""""herre
#include <readline/readline.h>
#include <readline/history.h>
# define BLUE "\033[94m"
# define RESET "\033[00m"
// struct sigaction sa;
# define BUFFER_SIZE 1024
// add >> <<

#define TOKEN_WORD     0
#define TOKEN_PIPE     1
#define TOKEN_INPUT    2
#define TOKEN_OUTPUT   3
#define TOKEN_APPEND   4
#define TOKEN_HEREDOC  5


typedef struct s_token
{
    int type;
    char *value;
}   t_token;

typedef struct s_command {
    char **args;
    char *input_file;
    char *output_file;
    int append_mode;
    int heredoc_mode;
    struct s_command *next;
} t_command;

typedef struct s_shell {
    t_token *tokens;
    t_command *commands;
    int token_count;
    char    **cmd;// containe arguments
    char    **copy_env;
    int exit_status;
    char **env;
    char    **export_env;
    char    *current_dir;
    char    **invalid_keys;
    int     invalid_count;
}   t_shell;
// typedef struct s_shell 
// {
//     char    **cmd;// containe arguments
//     int     exit_status;
    // char    **copy_env;
//     char    **export_env;
//     char    *current_dir;
//     char    **invalid_keys;
//     int     invalid_count;
// }   t_shell;


char        *get_env_value(t_shell *shell, const char *key);
int         process_heredoc(char **cmd);

void        sigint_handl(int sig);
void        sigquit_handler(int sig);

void        clean_tmp_files(int max_id);
char        **append_env(char **env, const char *new_var);
void        update_underscore_with_path(t_shell *shell, const char *full_path);

void        handle_shlvl(t_shell *shell);
int         ft_strcmp(const char *s1, const char *s2);

void        free_3d_array(char **arr);
int         exec_pipeline(char **cmds, t_shell *shell);
char        ***parse_pipes(char *line);


char        **copy_envp(char **envp);

void        free_array(char **av);
int         ft_exec(t_shell *shell);
char        **ft_token(char *line);
void        builtin_cd(t_shell *cmd);

void        builtin_pwd(t_shell *shell);
void        builtin_export(t_shell *shell);
void        builtin_echo(t_shell *cmd);
void	    builtin_env(t_shell *shell);

void        builtin_exit(t_shell *comd);

void        builtin_unset(t_shell *shell);
char        *get_path(t_shell *shell);
void        handl_builting(t_shell *shell);
int         is_builtin(char *cmd);

void        add_or_update_env(t_shell *shell, const char *arg);

int         contains_redirection(char **cmd);


long long   ft_atoll_with_overflow(const char *str, int *invalid);

int         handle_redirection(t_shell *shell, char **cmd);


/**what i add */
void parses(t_shell *cmd, char *input);
char	**ft_splited(const char *s);

#endif