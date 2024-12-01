#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char	**read_dictionary(char *path)
{
	int size = 100;
	char **dictionary = calloc(size, sizeof(char *));
	int i = 0;
	FILE *file = fopen(path, "r");
	size_t bytes = 0;
	while (getline(&dictionary[i], &bytes, file) > 0)
	{
		if (i == size)
		{
			size *= 2;
			dictionary = realloc(dictionary, size * sizeof(char *));
		}
		i++;
		dictionary[i] = NULL;
		bytes = 0;
	}
	fclose(file);
	return (dictionary);
}
