#include "main.h"

/**
 * _realloc - Reallocate memory for a block of memory.
 * @ptr: Pointer to the original memory block.
 * @old_size: Old size of the memory block.
 * @new_size: New size to reallocate.
 *
 * Description:
 * - This function reallocates a memory block pointed to by ptr to the new_size.
 * - If new_size is the same as old_size, the function returns the original ptr.
 * - If ptr is NULL and new_size is non-zero, the function behaves like malloc.
 * - If ptr is not NULL and new_size is 0, the function behaves like free.
 *
 * Return: Pointer to the reallocated memory block on success,
 *         NULL on failure or if @new_size is 0 (with ptr freed).
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * assign_lineptr - Assigns a new line pointer if necessary.
 * @lineptr: Pointer to the line pointer.
 * @u: Pointer to the size of the line.
 * @buffer: The buffer to assign to the line.
 * @q: The size of the buffer.
 *
 * Description:
 * - This function assigns a new line pointer to lineptr if the current lineptr
 *   is NULL or if the size of the buffer (q) is greater than the current size (*u).
 * - If a new lineptr is assigned, the old buffer is freed.
 * - If a new lineptr is not assigned, the contents of the buffer are copied to the line.
 */
void assign_lineptr(char **lineptr, size_t *u, char *buffer, size_t q)
{
	if (*lineptr == NULL)
	{
		if (q > 120)
			*u = q;
		else
			*u = 120;
		*lineptr = buffer;
	}
	else if (*u < q)
	{
		if (q > 120)
			*u = q;
		else
			*u = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - Read a line of input from the given stream.
 * @lineptr: Pointer to the line pointer where the input line will be stored.
 * @u: Pointer to the size of the line.
 * @stream: The input stream to read from (usually stdin).
 *
 * Description:
 * - This function reads a line of input from the given stream and stores it in the lineptr.
 * - It uses a static variable 'input' to keep track of the read position.
 * - If 'input' is 0, it flushes the stream. Otherwise, it returns -1 to indicate that
 *   the stream has already been read and prevents further reads.
 * - The input line is stored in a dynamically allocated buffer and assigned to lineptr.
 * - If the buffer size exceeds 120 characters, it is dynamically reallocated using _realloc.
 *
 * Return: The number of characters read, or -1 on failure or if the stream has been read.
 */
ssize_t _getline(char **lineptr, size_t *u, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int t;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		t = read(STDIN_FILENO, &c, 1);
		if (t == -1 || (t == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (t == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_lineptr(lineptr, u, buffer, input);

	ret = input;
	if (t != 0)
		input = 0;
	return (ret);
}
