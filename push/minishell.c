/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abrami <abrami@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:22:41 by abrami            #+#    #+#             */
/*   Updated: 2025/06/11 18:44:20 by abrami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/**
 * starting by parsing the input through splite but befor i check
 * if there's a pipe or heredoc if there's one of them
 * i parse witch is witch
 * then execut
 *   */ 


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
int	main(int ac, char **av, char **env)
{
	char		*input;
	t_shell     shell;

    (void)ac;
    (void)av;

    // initialize_shell(&shell);

    // shell.copy_env = copy_envp(env);
    // shell.exit_status = 0;
    // shell.invalid_keys = NULL;
    // shell.invalid_count = 0;
    // handle_shlvl(&shell);
    // signal(SIGINT, sigint_handl);
    // signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		input = readline("minishell!>> ");
        if (!input)
        {
            // if (cmd.args != NULL)
            //     free_char_array(cmd.args);
            printf("exit\n");
            rl_clear_history();
            exit(0);
        }
        if (input[0] == '\0') {
            free(input);
            continue;
        }
		if (*input)
			add_history(input);
		// if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
        // 	exit(1);
		// process_input(input, &shell);
        lexer(&shell, input);
        free(input);
        
        // Debug: print tokens
        print_tokens(&shell);

        // Parsing
        parser(&shell);
        
        // Debug: print parsed commands
        print_commands(&shell);
	}
    free_array(shell.copy_env);
	rl_clear_history();
	return (0);
}
