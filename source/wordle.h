#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>
#include <signal.h>
#include <ncurses.h>

#define WELCOME_ART "\
▗▖ ▗▖ ▗▄▖ ▗▄▄▖ ▗▄▄▄  ▗▖   ▗▄▄▄▖\n\
▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌  █ ▐▌   ▐▌   \n\
▐▌ ▐▌▐▌ ▐▌▐▛▀▚▖▐▌  █ ▐▌   ▐▛▀▀▘\n\
▐▙█▟▌▝▚▄▞▘▐▌ ▐▌▐▙▄▄▀ ▐▙▄▄▖▐▙▄▄▖\n\
"

#define WORD_LENGTH 5
#define MAX_GUESSES 6

#define COLOR_GREY 140

#define PLACEHOLDER           "_"
#define CHARACTER_SPACING     " "
#define GUESSES_INDENTATION   "           "
#define PROMPT                "      Guess: "
#define MSG_CORRECT           " You guessed '%s' correctly!\n"
#define MSG_GUESSES_NEEDED    "       Guesses needed: %d\n"
#define MSG_QUIT              "            Goodbye!\n"
#define MSG_OUT_OF_GUESSES    "    You ran out of guesses! The word was: %s\n"
#define MSG_INVALID_LENGTH    "      Not a 5 letter word!\n"
#define MSG_INVALID_CHARACTER "All characters have to be alphabetic!\n"
#define MSG_INVALID_WORD      " Word is not in the word list!\n"

typedef enum e_color
{
	GREY = 1,
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
bool prompt_input(char *input, int guess_count);
void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH]);
void print_grid(void);
void fill_row(char c);


void convert_input(const char *input, t_letter guess[WORD_LENGTH]);
void compare_guess(t_letter guess[WORD_LENGTH], t_word word);

#endif
