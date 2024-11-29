#include "wordle.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main(void)
{
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char *input = NULL;
	int guess_count = 0;

	while (guess_count < MAX_GUESSES /* && !is_game_finished() */)
	{
		// print_guesses(guesses);

		prompt_input(&input);
		if (!validate_input(input))
		{
			continue;
		}

		convert_input(input, guesses[guess_count]);
		// compare_guess(guesses[guess_count]);

		guess_count++;
	}
}
