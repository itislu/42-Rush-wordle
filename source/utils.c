#include <ctype.h>

void str_toupper(char *input)
{
	while (*input)
	{
		*input = toupper(*input);
		input++;
	}
}

int index_alpha(char c)
{
	if (islower(c))
	{
		return c - 'a';
	}
	if (isupper(c))
	{
		return c - 'A';
	}
	return -1;
}