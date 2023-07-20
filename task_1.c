#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/**
 * main - Program's entry point
 *
 * Description: This is a simple shell program responsible for
 * reading and executing user's single-word commands.
 *
 * Return: Always return 0 upon successful execution
 */
int main(void)
{
    char *prompt = "#My_Bot_says$ ";
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    pid_t pid;

    while (1)
    {
        printf("%s", prompt);
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes_read == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        else if (bytes_read == 0)
        {
            printf("\n");
            break;
        }

        buffer[bytes_read - 1] = '\0';

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char **args = malloc(sizeof(char *) * 2);
            if (args == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            args[0] = buffer;
            args[1] = NULL;

            if (execve(buffer, args, NULL) == -1)
            {
                perror(buffer);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    return (EXIT_SUCCESS);
}
