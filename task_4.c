#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ARG_SIZE 64
#define PATH_DELIMITER ":"

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
	char *prompt = "My Code Says: ";
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;
	pid_t pid;
	char *args[ARG_SIZE];
	char *token;
	int e_j;

	char *path;
	char *path_copy;
	char *path_token;
	char command_buffer[BUFFER_SIZE];
	int command_found;

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

		token = strtok(buffer, " ");
		e_j = 0;

		while (token != NULL && e_j < ARG_SIZE - 1)
		{
			args[e_j] = token;
			token = strtok(NULL, " ");
			e_j++;
		}

		args[e_j] = NULL;

		if (strcmp(args[0], "exit") == 0)
		{
			printf("You are exiting the shell.Peace be with you!\n");
			break;
		}

		/* Checking whether command is present in the PATH */
		path = getenv("PATH");
		path_copy = strdup(path);
		command_found = 0;

		path_token = strtok(path_copy, PATH_DELIMITER);
		while (path_token != NULL)
		{
			snprintf(command_buffer, BUFFER_SIZE, "%s/%s", path_token, args[0]);
			if (access(command_buffer, X_OK) == 0)
			{
				command_found = 1;
				break;
			}
			path_token = strtok(NULL, PATH_DELIMITER);
		}

		free(path_copy);

		if (command_found)
		{
			pid = fork();

			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				if (execve(command_buffer, args, NULL) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				waitpid(pid, NULL, 0);
			}
		}
		else
		{
			printf("The command not found: %s\n", args[0]);
		}
	}

	return (EXIT_SUCCESS);
}
