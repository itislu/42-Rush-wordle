#include "utils.h"
#include "wordle.h"
#include "words.h"
#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool is_valid_length(const char *input);
static bool is_alphabetic(const char *input);
static bool is_in_dictionary(const char *input);

bool prompt_input(char **input)
{
	free(*input);
	*input = readline("Enter your guess: ");
	if (*input == NULL)
	{
		return false;
	}
	if (**input != '\0')
	{
		add_history(*input);
	}
	str_toupper(*input);
	return true;
}

bool validate_input(const char *input)
{
	if (!is_valid_length(input))
	{
		printf("Not a 5 letter word!\n");
		return(false);
	}
	if (!is_alphabetic(input))
	{
		printf("All characters have to be alphabetic!\n");
		return(false);
	}
	if (!is_in_dictionary(input))
	{
		printf("Word is not in the word list!\n");
		return(false);
	}
	return (true);
}

static bool is_valid_length(const char *input)
{
	int i = strlen(input);
	if (i == WORD_LENGTH)
		return (true);
	return (false);
}
static bool is_alphabetic(const char *input)
{
	int i = 0;
	while (input[i])
	{
		if (!isalpha(input[i]))
			return (false);
		i++;
	}
	return (true);
}
static bool is_in_dictionary(const char *input)
{
	char word[WORD_LENGTH + 1] = {'\0'};
	int i = 0;
	while (g_dict[i])
	{
		memcpy(word, g_dict[i], WORD_LENGTH);
		str_toupper(word);
		if (strcmp(input, word) == 0)
			return (true);
		i++;
	}
	return (false);
}

/* 
int main()
{
	const char *word = "housl";
	printf("%d\n", is_valid_length(word));
	printf("%d\n", is_alphabetic(word));
	printf("%d\n", is_in_dictionary(word));
	printf("%d\n", validate_input(word));
	for(int i = 0; word[i]; i++)
	{
		printf("%c", word[i]);
	}
} */