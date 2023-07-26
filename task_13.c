#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 10
#define MAX_ALIASES 100

struct Alias {
    char *name;
    char *value;
};

struct Alias alias_list[MAX_ALIASES];
int num_aliases = 0;

void split_line(char *line, char **args) {
    int e_j = 0;
    char *token = strtok(line, " \t\n\r");

    while (token != NULL && e_j < MAX_ARGS - 1) {
        args[e_j] = token;
        token = strtok(NULL, " \t\n\r");
        e_j++;
    }

    args[e_j] = NULL;
}

void print_aliases() {
    for (int e_j = 0; e_j < num_aliases; e_j++) {
        printf("%s='%s'\n", alias_list[e_j].name, alias_list[e_j].value);
    }
}

int find_alias_index(char *name) {
    for (int e_j = 0; e_j < num_aliases; e_j++) {
        if (strcmp(alias_list[e_j].name, name) == 0) {
            return e_j;
        }
    }
    return -1;
}

void execute_alias(char **args) {
    if (args[1] == NULL) {
        print_aliases();
    } else {
        for (int e_j = 1; args[e_j] != NULL; e_j++) {
            char *name = args[e_j];
            char *value = NULL;

            char *equal_sign = strchr(args[e_j], '=');
            if (equal_sign != NULL) {
                *equal_sign = '\0';
                value = equal_sign + 1;
            }

            int alias_index = find_alias_index(name);
            if (alias_index != -1) {
                free(alias_list[alias_index].value);
                alias_list[alias_index].value = strdup(value);
            } else {
                if (num_aliases < MAX_ALIASES) {
                    alias_list[num_aliases].name = strdup(name);
                    alias_list[num_aliases].value = strdup(value);
                    num_aliases++;
                } else {
                    printf("Alias list is full, cannot add more aliases.\n");
                }
            }
        }
    }
}

void execute_exit() {
    for (int e_j = 0; e_j < num_aliases; e_j++) {
        free(alias_list[e_j].name);
        free(alias_list[e_j].value);
    }
    exit(0);
}

void execute_logical_operators(char **args);

void execute_command(char **args) {
    int e_j = 0;
    char *command = args[e_j];
    pid_t pid;
    int status;

    int alias_index = find_alias_index(command);
    if (alias_index != -1) {
        char *alias_value = alias_list[alias_index].value;
        char *token = strtok(alias_value, " \t\n\r");
        while (token != NULL && e_j < MAX_ARGS - 1) {
            args[e_j] = token;
            token = strtok(NULL, " \t\n\r");
            e_j++;
        }
        args[e_j] = NULL;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        execvp(args[e_j], args);

        perror("execvp");
        exit(1);
    } else {
        waitpid(pid, &status, 0);
    }
}

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("Shell> ");
        fflush(stdout);

        if (fgets(line, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;
        }

        split_line(line, args);

        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                execute_exit();
            } else if (strcmp(args[0], "alias") == 0) {
                execute_alias(args);
            } else {
                execute_logical_operators(args);
            }
        }
    }

    return 0;
}
