#include "utils.h"
#include "wordle.h"

static void set_green(t_letter guess[WORD_LENGTH], t_word *word);
static void set_yellow(t_letter guess[WORD_LENGTH], t_word *word);

void convert_input(const char *input, t_letter guess[WORD_LENGTH])
{
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		guess[i].letter = input[i];
		guess[i].color = GREY;
	}
}

void compare_guess(t_letter guess[WORD_LENGTH], t_word word)
{
	// int alphabet[26];
	// memcpy(alphabet, word->alphabet, sizeof(alphabet));

	set_green(guess, &word);
	set_yellow(guess, &word);
}

static void set_green(t_letter guess[WORD_LENGTH], t_word *word)
{
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guess[i].letter == word->word[i])
		{
			guess[i].color = GREEN;
			word->alphabet[index_alpha(guess[i].letter)]--;
		}
	}
}

static void set_yellow(t_letter guess[WORD_LENGTH], t_word *word)
{
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guess[i].color == GREEN || word->alphabet[index_alpha(guess[i].letter)] == 0)
		{
			continue;
		}
		guess[i].color = YELLOW;
		word->alphabet[index_alpha(guess[i].letter)]--;
	}
}
