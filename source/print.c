#include "wordle.h"
#include <stdio.h>

static void print_letter(t_letter letter);

void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH])
{
	printf("\n");
	for (int i = 0; i < MAX_GUESSES; i++)
	{
		printf(GUESSES_INDENTATION);
		for (int j = 0; j < WORD_LENGTH; j++)
		{
			print_letter(guesses[i][j]);
			printf(CHARACTER_SPACING);
		}
		printf("\n");
	}
	printf("\n");
}

static void print_letter(t_letter letter)
{
	char *color;
	char c = letter.letter;

	if (c == '\0')
	{
		printf(PLACEHOLDER);
		return;
	}
	switch (letter.color)
	{
		case GREEN:
			color = COLOR_GREEN;
			break;
		case YELLOW:
			color = COLOR_YELLOW;
			break;
		case GREY:
			color = COLOR_GREY;
			break;
	}
	printf("%s%c%s", color, c, COLOR_RESET);
}
