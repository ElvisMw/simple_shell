#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 10

/**
 * split_line - Split input line into singular arguments.
 * @line: User's input line
 * @args: Stores the arguments
 */
void split_line(char *line, char **args)
{
	int e_j = 0;
	char *token = strtok(line, " \t\n\r");

	while (token != NULL && e_j < MAX_ARGS - 1)
	{
		args[e_j] = token;
		token = strtok(NULL, " \t\n\r");
		e_j++;
	}

	args[e_j] = NULL;
}

/**
 * execute_exit - Execute the "exit" command for shell termination
 */
void execute_exit(void)
{
	exit(0);
}

/**
 * execute_setenv - Execute the "setenv" command for environment variables
 * @args: Array of strings containing command  arguments
 */
void execute_setenv(char **args)
{
	if (args[1] && args[2])
	{
		if (setenv(args[1], args[2], 1) != 0)
		{
			perror("setenv");
		}
	}
	else
	{
		printf("Usage: setenv VARIABLE VALUE\n");
	}
}

/**
 * execute_unsetenv - Run "unsetenv" to unset environment variables
 * @args: Array of strings containing command arguments
 */
void execute_unsetenv(char **args)
{
	if (args[1])
	{
		if (unsetenv(args[1]) != 0)
		{
			perror("unsetenv");
		}
	}
	else
	{
		printf("Usage: unsetenv VARIABLE\n");
	}
}

/**
 * execute_cd - Perfom"cd" command to change the current directory
 * @args: Array of strings containing command arguments
 */
void execute_cd(char **args)
{
	if (args[1])
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
		}
	}
	else
	{
		printf("Usage: cd DIRECTORY\n");
	}
}

/**
 * execute_logical_operators - run commands with logical operators(&& and ||)
 * @args: String array command arguments
 */
void execute_logical_operators(char **args)
{
	int e_j = 0;
	int status;

	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		/* Child_process */
		execvp(args[e_j], args);

		perror("execvp");
		exit(1);
	}
	else
	{
		/* Parent_process */
		waitpid(pid, &status, 0);

		if (args[e_j + 1])
		{
			if (strcmp(args[e_j + 1], "&&") == 0)
			{
				if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
				{
					execute_logical_operators(&args[e_j + 2]);
				}
			}
			else if (strcmp(args[e_j + 1], "||") == 0)
			{
				if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
				{
					/* ifprevious command has failed, begin executing the next command */
					execute_logical_operators(&args[e_j + 2]);
				}
			}
		}
	}
}

/**
 * execute_command - Run a single command.
 * @args: String array for command arguments
 */
void execute_command(char **args)
{
	int e_j = 0;
	pid_t pid;
	int status;

	pid = fork();

	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		/* Child_process */
		execvp(args[e_j], args);

		perror("execvp");
		exit(1);
	}
	else
	{
		/* Parent_proces_s */
		waitpid(pid, &status, 0);
	}
}

int main(void)
{
	char line[MAX_LINE];
	char *args[MAX_ARGS];

	while (1)
	{
		printf("Shell> ");
		fflush(stdout);

		if (fgets(line, MAX_LINE, stdin) == NULL)
		{
			printf("\n");
			break;
		}

		split_line(line, args);

		if (args[0] != NULL)
		{
			if (strcmp(args[0], "exit") == 0)
			{
				execute_exit();
			}
			else if (strcmp(args[0], "setenv") == 0)
			{
				execute_setenv(args);
			}
			else if (strcmp(args[0], "unsetenv") == 0)
			{
				execute_unsetenv(args);
			}
			else if (strcmp(args[0], "cd") == 0)
			{
				execute_cd(args);
			}
			else
			{
				execute_logical_operators(args);
			}
		}
	}

	return (0);
}
