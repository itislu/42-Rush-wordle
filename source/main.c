#include "wordle.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], 
	int guess_count, 
	const t_word *word);

int main(void)
{
	t_word word = {0};
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char *input = NULL;
	int guess_count = 0;

	printf("%s\n", WELCOME_ART);
	pick_word(&word);
	while (!is_game_finished(guesses, guess_count, &word))
	{
		if (!prompt_input(&input))
		{
			printf("Goodbye!\n");
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
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], 
	int guess_count, 
	const t_word *word)
{
	print_guesses(guesses);
	if (guess_count == 0)
	{
		return false;
	}
	if (guess_count >= MAX_GUESSES)
	{
		printf("You ran out of guesses!\n");
		return true;
	}
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guesses[guess_count - 1][i].color != GREEN)
		{
			return false;
		}
	}
	printf("You guessed '%s' correctly!\n", word->word);
	printf("Guesses needed: %d\n", guess_count);
	return true;
}
