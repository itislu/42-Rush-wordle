#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define WELCOME_ART "\
 .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------. \n\
| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |\n\
| | _____  _____ | || |     ____     | || |  _______     | || |  ________    | || |   _____      | || |  _________   | |\n\
| ||_   _||_   _|| || |   .'    `.   | || | |_   __ \\    | || | |_   ___ `.  | || |  |_   _|     | || | |_   ___  |  | |\n\
| |  | | /\\ | |  | || |  /  .--.  \\  | || |   | |__) |   | || |   | |   `. \\ | || |    | |       | || |   | |_  \\_|  | |\n\
| |  | |/  \\| |  | || |  | |    | |  | || |   |  __ /    | || |   | |    | | | || |    | |   _   | || |   |  _|  _   | |\n\
| |  |   /\\   |  | || |  \\  `--'  /  | || |  _| |  \\ \\_  | || |  _| |___.' / | || |   _| |__/ |  | || |  _| |___/ |  | |\n\
| |  |__/  \\__|  | || |   `.____.'   | || | |____| |___| | || | |________.'  | || |  |________|  | || | |_________|  | |\n\
| |              | || |              | || |              | || |              | || |              | || |              | |\n\
| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |\n\
 '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' \n\
"

#define WORD_LENGTH 5
#define MAX_GUESSES 6

#define COLOR_RESET "\e[0m"
#define COLOR_GREY "\e[1;38;2;255;255;255;48;2;58;58;60m"
#define COLOR_YELLOW "\e[1;38;2;255;255;255;48;2;180;164;60m"
#define COLOR_GREEN "\e[1;38;2;255;255;255;48;2;83;141;78m"

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
