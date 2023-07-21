#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ARG_SIZE 64
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
	char *prompt = "#MY CODE SAYS$ ";
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
			char *args[ARG_SIZE];
			char *token;
			int e_j = 0;

			token = strtok(buffer, " ");
			while (token != NULL && e_j < ARG_SIZE - 1)
			{
				args[e_j] = token;
				token = strtok(NULL, " ");
				e_j++;
			}
			args[e_j] = NULL;

			if (execvp(args[0], args) == -1)
			{
				perror(args[0]);
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
