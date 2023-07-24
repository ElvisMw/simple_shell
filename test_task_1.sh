#!/bin/bash

# For compiling this shell program
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 task_1.c -o shell_task_1

# Test case 1: ls input
echo -e "\nTest Case 1: ls command"
echo "#My_Bot_says$ ls" | ./shell_task_1

# Test case 2: pwd input command
echo -e "\nTest Case 2: pwd command"
echo "#My_Bot_says$ pwd" | ./shell_task_1

# Test case 3: Not available command
echo -e "\nTest Case 3: Non-existing command"
echo "#My_Bot_says$ nonexistent_command" | ./shell_task_1

# Test case 4: Exit command
echo -e "\nTest Case 4: Exit command"
echo "#My_Bot_says$ exit" | ./shell_task_1

# Clean up
rm -f shell_task_1

