#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

extern int g_var;  // Ensure g_var is declared elsewhere in your program
pid_t child_pid = -1;  // Store the child PID for sending signals

// Signal handler function
void lis(int i)
{
    if (i == SIGQUIT) {
        // If Ctrl+\ (SIGQUIT) is pressed, forward SIGQUIT to the child process
        if (child_pid > 0) {
            printf("\nSending SIGQUIT to child process (PID: %d)\n", child_pid);
            kill(child_pid, SIGQUIT);  // Forward SIGQUIT to the child process
        }
        return;
    }

    // Handle SIGINT (Ctrl-C)
    if (g_var == 1)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}


// Function to simulate executing a command
void execute_command(char *command)
{
    child_pid = fork();

    if (child_pid == 0) {
        // In child process: execute the command
        execlp(command, command, NULL);
        perror("exec failed");
        exit(1);
    } else if (child_pid > 0) {
        // In parent process: wait for the child to finish
        int status;
        waitpid(child_pid, &status, 0);
        child_pid = -1;  // Reset child_pid after the child finishes
    }
}

int main()
{
    ft_sigaction();  // Set up signal handlers

    while (1) {
        // Read user input (command to execute)
        char *input = readline("mysh> ");
        if (input == NULL) break;  // Exit if input is NULL (Ctrl-D)

        // Execute the command (e.g., cat, sleep, etc.)
        execute_command(input);
        free(input);  // Free input memory
    }

    return 0;
}
