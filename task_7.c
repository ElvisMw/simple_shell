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
