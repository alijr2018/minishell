#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

#define TOKEN_WORD     0
#define TOKEN_PIPE     1
#define TOKEN_INPUT    2
#define TOKEN_OUTPUT   3
#define TOKEN_APPEND   4
#define TOKEN_HEREDOC  5

typedef struct s_token {
    int type;
    char *value;
} t_token;

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
    int exit_status;
    char **env;
} t_shell;

// Function prototypes
void lexer(t_shell *shell, const char *input);
void parser(t_shell *shell);
void execute(t_shell *shell);
void free_shell(t_shell *shell);
void print_tokens(t_shell *shell);
void print_commands(t_shell *shell);

int main() {
    t_shell shell = {0};
    char *input;

    printf("Minishell with Advanced Lexer/Parser\n");
    printf("Type 'exit' or 'quit' to exit\n");

    while (1) {
        input = readline("minishell> ");
        if (!input) {
            printf("exit\n");
            break;
        }

        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        add_history(input);

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Lexing (tokenization)
        lexer(&shell, input);
        free(input);
        
        // Debug: print tokens
        print_tokens(&shell);

        // Parsing
        parser(&shell);
        
        // Debug: print parsed commands
        print_commands(&shell);

        // Execution
        execute(&shell);

        // Cleanup
        free_shell(&shell);
    }

    return shell.exit_status;
}

void lexer(t_shell *shell, const char *input) {
    // First pass: count tokens
    shell->token_count = 0;
    const char *p = input;
    while (*p) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        shell->token_count++;
        
        if (*p == '|') { p++; continue; }
        if (*p == '<') { p++; if (*p == '<') p++; continue; }
        if (*p == '>') { p++; if (*p == '>') p++; continue; }
        
        while (*p && !isspace(*p)) {
            if (*p == '\'' || *p == '"') {
                char quote = *p++;
                while (*p && *p != quote) p++;
                if (*p) p++;
            } else if (*p == '|' || *p == '<' || *p == '>') {
                break;
            } else {
                p++;
            }
        }
    }

    // Allocate tokens
    shell->tokens = malloc((shell->token_count + 1) * sizeof(t_token));
    
    // Second pass: create tokens
    int i = 0;
    p = input;
    while (*p) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        
        if (*p == '|') {
            shell->tokens[i].type = TOKEN_PIPE;
            shell->tokens[i].value = strdup("|");
            p++;
            i++;
            continue;
        }
        
        if (*p == '<') {
            if (p[1] == '<') {
                shell->tokens[i].type = TOKEN_HEREDOC;
                shell->tokens[i].value = strdup("<<");
                p += 2;
            } else {
                shell->tokens[i].type = TOKEN_INPUT;
                shell->tokens[i].value = strdup("<");
                p++;
            }
            i++;
            continue;
        }
        
        if (*p == '>') {
            if (p[1] == '>') {
                shell->tokens[i].type = TOKEN_APPEND;
                shell->tokens[i].value = strdup(">>");
                p += 2;
            } else {
                shell->tokens[i].type = TOKEN_OUTPUT;
                shell->tokens[i].value = strdup(">");
                p++;
            }
            i++;
            continue;
        }
        
        // Handle words (including quoted strings)
        shell->tokens[i].type = TOKEN_WORD;
        char *word = malloc(256);
        int word_pos = 0;
        
        while (*p && !isspace(*p)) {
            if (*p == '\'' || *p == '"') {
                char quote = *p++;
                while (*p && *p != quote) {
                    word[word_pos++] = *p++;
                }
                if (*p) p++;
            } else if (*p == '|' || *p == '<' || *p == '>') {
                break;
            } else {
                word[word_pos++] = *p++;
            }
        }
        
        word[word_pos] = '\0';
        shell->tokens[i].value = word;
        i++;
    }
    
    // Add EOF marker
    shell->tokens[i].type = -1;
    shell->tokens[i].value = NULL;
}

void parser(t_shell *shell) {
    t_command *cmd = NULL;
    t_command **tail = &shell->commands;
    int i = 0;
    
    while (i < shell->token_count) {
        if (cmd == NULL) {
            cmd = malloc(sizeof(t_command));
            cmd->args = NULL;
            cmd->input_file = NULL;
            cmd->output_file = NULL;
            cmd->append_mode = 0;
            cmd->heredoc_mode = 0;
            cmd->next = NULL;
            *tail = cmd;
            tail = &cmd->next;
        }
        
        if (shell->tokens[i].type == TOKEN_WORD) {
            // Count arguments
            int arg_count = 0;
            int j = i;
            while (j < shell->token_count && shell->tokens[j].type == TOKEN_WORD) {
                arg_count++;
                j++;
            }
            
            // Allocate and fill arguments
            cmd->args = malloc((arg_count + 1) * sizeof(char *));
            for (int k = 0; k < arg_count; k++) {
                cmd->args[k] = strdup(shell->tokens[i + k].value);
            }
            cmd->args[arg_count] = NULL;
            i += arg_count;
        }
        else if (shell->tokens[i].type == TOKEN_INPUT) {
            if (i + 1 >= shell->token_count || shell->tokens[i + 1].type != TOKEN_WORD) {
                fprintf(stderr, "Syntax error near '<'\n");
                shell->exit_status = 1;
                return;
            }
            cmd->input_file = strdup(shell->tokens[i + 1].value);
            i += 2;
        }
        else if (shell->tokens[i].type == TOKEN_OUTPUT) {
            if (i + 1 >= shell->token_count || shell->tokens[i + 1].type != TOKEN_WORD) {
                fprintf(stderr, "Syntax error near '>'\n");
                shell->exit_status = 1;
                return;
            }
            cmd->output_file = strdup(shell->tokens[i + 1].value);
            cmd->append_mode = 0;
            i += 2;
        }
        else if (shell->tokens[i].type == TOKEN_APPEND) {
            if (i + 1 >= shell->token_count || shell->tokens[i + 1].type != TOKEN_WORD) {
                fprintf(stderr, "Syntax error near '>>'\n");
                shell->exit_status = 1;
                return;
            }
            cmd->output_file = strdup(shell->tokens[i + 1].value);
            cmd->append_mode = 1;
            i += 2;
        }
        else if (shell->tokens[i].type == TOKEN_HEREDOC) {
            if (i + 1 >= shell->token_count || shell->tokens[i + 1].type != TOKEN_WORD) {
                fprintf(stderr, "Syntax error near '<<'\n");
                shell->exit_status = 1;
                return;
            }
            cmd->input_file = strdup(shell->tokens[i + 1].value);
            cmd->heredoc_mode = 1;
            i += 2;
        }
        else if (shell->tokens[i].type == TOKEN_PIPE) {
            i++;
            cmd = NULL;  // Ready for next command
        }
    }
}

void execute(t_shell *shell) {
    t_command *cmd = shell->commands;
    int prev_pipe[2] = {-1, -1};
    int next_pipe[2];
    
    while (cmd) {
        // Setup pipes if there's another command after this one
        if (cmd->next) {
            if (pipe(next_pipe) == -1) {
                perror("pipe");
                shell->exit_status = 1;
                return;
            }
        }
        
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            
            // Handle input redirection
            if (cmd->input_file) {
                if (cmd->heredoc_mode) {
                    // Heredoc implementation would go here
                    fprintf(stderr, "Heredoc not implemented yet\n");
                    exit(1);
                } else {
                    int fd = open(cmd->input_file, O_RDONLY);
                    if (fd == -1) {
                        perror("open");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
            } else if (prev_pipe[0] != -1) {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }
            
            // Handle output redirection
            if (cmd->output_file) {
                int flags = O_WRONLY | O_CREAT;
                if (cmd->append_mode) {
                    flags |= O_APPEND;
                } else {
                    flags |= O_TRUNC;
                }
                int fd = open(cmd->output_file, flags, 0644);
                if (fd == -1) {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (cmd->next) {
                dup2(next_pipe[1], STDOUT_FILENO);
                close(next_pipe[0]);
                close(next_pipe[1]);
            }
            
            // Execute command
            if (cmd->args && cmd->args[0]) {
                execvp(cmd->args[0], cmd->args);
                perror("execvp");
                exit(1);
            }
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            shell->exit_status = 1;
            return;
        }
        
        // Close previous pipe in parent
        if (prev_pipe[0] != -1) {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }
        
        // Save current pipe for next iteration
        if (cmd->next) {
            prev_pipe[0] = next_pipe[0];
            prev_pipe[1] = next_pipe[1];
        } else {
            prev_pipe[0] = -1;
            prev_pipe[1] = -1;
        }
        
        cmd = cmd->next;
    }
    
    // Wait for all child processes
    while (wait(NULL) > 0);
}

void free_shell(t_shell *shell) {
    // Free tokens
    for (int i = 0; i < shell->token_count; i++) {
        free(shell->tokens[i].value);
    }
    free(shell->tokens);
    shell->tokens = NULL;
    shell->token_count = 0;
    
    // Free commands
    t_command *cmd = shell->commands;
    while (cmd) {
        t_command *next = cmd->next;
        
        // Free arguments
        if (cmd->args) {
            for (int i = 0; cmd->args[i]; i++) {
                free(cmd->args[i]);
            }
            free(cmd->args);
        }
        
        free(cmd->input_file);
        free(cmd->output_file);
        free(cmd);
        cmd = next;
    }
    shell->commands = NULL;
}

void print_tokens(t_shell *shell) {
    printf("Tokens:\n");
    for (int i = 0; i < shell->token_count; i++) {
        const char *type_str;
        switch (shell->tokens[i].type) {
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_PIPE: type_str = "PIPE"; break;
            case TOKEN_INPUT: type_str = "INPUT"; break;
            case TOKEN_OUTPUT: type_str = "OUTPUT"; break;
            case TOKEN_APPEND: type_str = "APPEND"; break;
            case TOKEN_HEREDOC: type_str = "HEREDOC"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("  %s: '%s'\n", type_str, shell->tokens[i].value);
    }
}

void print_commands(t_shell *shell) {
    printf("Parsed Commands:\n");
    t_command *cmd = shell->commands;
    while (cmd) {
        printf("Command:\n");
        if (cmd->args) {
            printf("  Args:");
            for (int i = 0; cmd->args[i]; i++) {
                printf(" '%s'", cmd->args[i]);
            }
            printf("\n");
        }
        if (cmd->input_file) {
            printf("  Input: '%s' (%s)\n", cmd->input_file, 
                   cmd->heredoc_mode ? "heredoc" : "file");
        }
        if (cmd->output_file) {
            printf("  Output: '%s' (%s)\n", cmd->output_file,
                   cmd->append_mode ? "append" : "truncate");
        }
        cmd = cmd->next;
    }
}