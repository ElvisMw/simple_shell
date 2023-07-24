#!/bin/bash

# For compilation of shell program
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 task_2.c -o shell_task_2

# 1st test case: user input ls command
echo -e "\n1st test Case: ls command"
echo "#MY CODE SAYS$ ls" | ./shell_task_2

# 2nd test case: user input is pwd command
echo -e "\n2nd test Case: pwd command"
echo "#MY CODE SAYS$ pwd" | ./shell_task_2

# 3rd test case: user input is Non-existing command
echo -e "\n3rd test case: Non-existing command"
echo "#MY CODE SAYS$ nonexistent_command" | ./shell_task_2

# 4th test case: Exit command
echo -e "\n4th test Case: Exit command"
echo "#MY CODE SAYS$ exit" | ./shell_task_2

# Clean up
rm -f shell_task_2
