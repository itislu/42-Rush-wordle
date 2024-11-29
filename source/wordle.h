#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6

#define COLOR_RESET "\e0m"
#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_GREY "\e[90m"

typedef enum e_color
{
	GREY,
	YELLOW,
	GREEN
} t_color;

typedef struct s_letter {
	char letter;
	t_color color;
} t_letter;

bool validate_input(const char *input);
void prompt_input(char **input);
void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH]);

void convert_input(const char *input, t_letter guess[WORD_LENGTH]);

#endif
