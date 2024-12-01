#include "wordle.h"
#include <stdio.h>
#include <ncurses.h>

static void print_letter(t_letter letter);
// static void print_current_row_letter(t_letter letter, int guess_count, int position);

void print_grid(void)
{
	for (int i = 0; i < MAX_GUESSES; i++)
	{
		move(i, 0);
		fill_row('_');
	}
}

void fill_row(char c)
{
	int x;
	int y;
	getyx(stdscr, y, x);
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		mvprintw(y, i + i, "%c", c);
	}
	move(y, x);
}

void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH])
{
	for (int i = 0; guesses[i][0].letter != '\0'; i++)
	{
		move(i, 0);
		for (int j = 0; j < WORD_LENGTH; j++)
		{
			// move(i, j * 2);
			print_letter(guesses[i][j]);
			printw(" ");
		}
	}
}

static void print_letter(t_letter letter)
{
	int color;
	char c = letter.letter;

	if (c == '\0')
	{
		printw("_");
		return;
	}
	switch (letter.color)
	{
		case GREEN:
			color = COLOR_PAIR(GREEN);
			break;
		case YELLOW:
			color = COLOR_PAIR(YELLOW);
			break;
		case GREY:
			color = COLOR_PAIR(GREY);
			break;
	}
	attron(color | A_BOLD);
	printw("%c", c);
	attroff(color | A_BOLD);
}
/* 
void print_current_row(const t_letter guesses[MAX_GUESSES][WORD_LENGTH], int guess_count)
{
	for (int j = 0; j < WORD_LENGTH; j++)
		{
			print_current_row_letter(guesses[guess_count][j], guess_count, j);
			refresh();
			//printf(" ");
		}
		//printf("\n");
		move(7, 0);
}

static void print_current_row_letter(t_letter letter, int guess_count, int position)
{
	char *color;
	char c = letter.letter;

	if (c == '\0')
	{
		printf("_");
		return;
	}
	switch (letter.color)
	{
		case GREEN:
			color = "\033[1;38;2;255;255;255;48;2;83;141;78m";
			break;
		case YELLOW:
			color = "\033[1;38;2;255;255;255;48;2;180;164;60m";
			break;
		case GREY:
			color = CLR_GREY;
			break;
	}
	printf("%s%c%s", color, c, CLR_RESET);
	fflush(stdout);
	move(guess_count, position + position);
}
 */