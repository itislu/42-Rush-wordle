#include "wordle.h"

void convert_input(const char *input, t_letter guess[WORD_LENGTH])
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		guess->letter = input[i];
		guess->color = GREY;
	}
}

// compare_guess(guesses[guess_count]);
