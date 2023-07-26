#include "main.h"

/**
 * e_j_alias - Builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1. Otherwise - 0.
 */
int e_j_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);
void print_alias(alias_t *alias);

int e_j_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int m_m, ret = 0;
	char *value;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}
	for (m_m = 0; args[m_m]; m_m++)
	{
		temp = aliases;
		value = _strchr(args[m_m], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[m_m], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = create_error(args + m_m, 1);
		}
		else
			set_alias(args[m_m], value);
	}
	return (ret);
}

void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int len, j, k;
	char *new_value;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}

void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}

char **replace_aliases(char **args)
{
	alias_t *temp;
	int m_m;
	char *new_value;

	if (_strcmp(args[0], "e_j") == 0)
		return (args);
	for (m_m = 0; args[m_m]; m_m++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[m_m], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[m_m]);
				args[m_m] = new_value;
				m_m--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}
