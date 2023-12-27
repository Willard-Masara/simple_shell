#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

/* command_executer - executes command using execve
 * @command: The command to execute
 *
 * Return: No return value
 */
void command_executer(char *command) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0) {
        /*Child process*/
        char *args[2];
        args[0] = command;
        args[1] = NULL;

        execve(command, args, NULL);

        /*execve only returns on error*/
        perror("Execution failed");
        exit(1);
    } else {
        /*Parent process*/
        waitpid(child_pid, NULL, 0);
    }
}
/* prompt - displays prompt and waits for user input
 *
 * Return: No return value
 */
void prompt() {
    char string[] = "cisfun$ ";
    char *input = NULL;
    size_t buf_size = 0;
    ssize_t read_bytes;

    while (1) {
        /*Display prompt*/ 
        write(1, string, sizeof(string) - 1);

        /* Read user input*/
        read_bytes = getline(&input, &buf_size, stdin);

        if (read_bytes == -1) {
            perror("Error reading input");
            exit(1);
        }

        /*Remove newline character*/
        if (input[read_bytes - 1] == '\n') {
            input[read_bytes - 1] = '\0';
        }

        /*Check for end of file (Ctrl+D)*/
        if (read_bytes == EOF) {
            write(1, "\n", 1);
            free(input);
            exit(0);
        }

        /*Execute the command*/
        command_executer(input);
    }
}

/* main - entry point of the program
 * @argc: Number of command line arguments
 * @argv: Array of command line arguments
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{

        int i;

        if (argc > 1) {
        /* Non-interactive mode*/
        for (i = 1; i < argc; ++i) {
            command_executer(argv[i]);
        }
    } else {
        /*Interactive mode*/
        prompt();
    }

    return 0;
}  
