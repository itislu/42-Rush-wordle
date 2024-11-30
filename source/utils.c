#include <ctype.h>

void str_tolower(char *input)
{
	while (*input)
	{
		*input = tolower(*input);
		input++;
	}
}

int index_alpha(char c)
{
	return c - 'a';
}