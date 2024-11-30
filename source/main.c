#include "wordle.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], int guess_count);

int main(void)
{
	t_word word = {0};
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char *input = NULL;
	int guess_count = 0;

	pick_word(&word);
	while (!is_game_finished(guesses, guess_count))
	{
		// print_guesses(guesses);

		if (!prompt_input(&input))
		{
			break;
		}
		if (!validate_input(input))
		{
			continue;
		}

		convert_input(input, guesses[guess_count]);
		compare_guess(guesses[guess_count], word);

		guess_count++;
	}
	free(word.word);
}

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], int guess_count)
{
	if (guess_count == 0)
	{
		return false;
	}
	if (guess_count >= MAX_GUESSES)
	{
		return true;
	}
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guesses[guess_count - 1][i].color != GREEN)
		{
			return false;
		}
	}
	return true;
}
