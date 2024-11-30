#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6

// We can do the exact wordle colors
#define COLOR_RESET "\e[0m"
#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_GREY "\e[90m"

typedef enum e_color
{
	GREY,
	YELLOW,
	GREEN
} t_color;

typedef struct s_letter 
{
	char letter;
	t_color color;
} t_letter;

typedef struct s_word 
{
	char *word;
	int alphabet[26];
} t_word;

void pick_word(t_word *word);

bool validate_input(const char *input);
bool prompt_input(char **input);
void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH]);

void convert_input(const char *input, t_letter guess[WORD_LENGTH]);
void compare_guess(t_letter guess[WORD_LENGTH], t_word word);

#endif
