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
 * custom_getline - Reads user's line of input.
 *
 * Description: Reads characters from standard input, till newline character
 * or full buffer are realized.
 *
 * Return: User's input line (null-terminated string).
 */
char *custom_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static ssize_t pos = 0;
	static ssize_t bytes_read = 0;
	char *line;
	ssize_t e_j;

	while (1)
	{
		if (pos >= bytes_read)
		{
			bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			if (bytes_read == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			if (bytes_read == 0)
				return (NULL);
			pos = 0;
		}

		for (e_j = pos; e_j < bytes_read; e_j++)
		{
			if (buffer[e_j] == '\n')
			{
				line = malloc((e_j - pos + 1) * sizeof(char));
				if (line == NULL)
				{
					perror("malloc");
					exit(EXIT_FAILURE);
				}
				memcpy(line, buffer + pos, e_j - pos);
				line[e_j - pos] = '\0';
				pos = e_j + 1;
				return (line);
			}
		}

		if (bytes_read - pos >= BUFFER_SIZE)
		{
			line = malloc(BUFFER_SIZE * sizeof(char));
			if (line == NULL)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			memcpy(line, buffer + pos, BUFFER_SIZE);
				pos += BUFFER_SIZE;
			return (line);
		}
	}
}

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
	char *prompt = "$ ";
	char *line;
	char *token;
	char *args[ARG_SIZE];
	int e_j;

	char *path;
	char *path_copy;
	char *path_token;
	char command_buffer[BUFFER_SIZE];
	int command_found;

	while (1)
	{
		printf("%s", prompt);
		line = custom_getline();

		if (line == NULL)
		{
			printf("\n");
			break;
		}

		token = strtok(line, " ");
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
		else
		{
			/* Checking whether the command exists in the PATH */
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
				pid_t pid = fork();

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
				printf("The command entered is not available: %s\n", args[0]);
			}
		}

		free(line);
	}

	return (EXIT_SUCCESS);
}
