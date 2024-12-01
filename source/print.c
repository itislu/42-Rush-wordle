#include "wordle.h"
#include <ncursesw/curses.h>
#include <unistd.h>

static void print_letter(t_letter letter);

void print_grid(void)
{
	for (int i = 0; i < MAX_GUESSES; i++)
	{
		move(i + ROW_OFFSET, GUESSES_INDENTATION);
		fill_row();
	}
}

void fill_row()
{
	int x;
	int y;
	getyx(stdscr, y, x);
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		mvprintw(y, i + i + GUESSES_INDENTATION, "_");
	}
	move(y, x);
}

void print_guesses(const t_letter guesses[MAX_GUESSES][WORD_LENGTH], int guess_count)
{
	curs_set(0);
	for (int i = 0; i < guess_count; i++)
	{
		move(i + ROW_OFFSET, GUESSES_INDENTATION);
		for (int j = 0; j < WORD_LENGTH; j++)
		{
			print_letter(guesses[i][j]);
			printw(CHARACTER_SPACING);
			if (i + 1 == guess_count)
			{
				usleep(ANIMATION_SPEED_US);
				refresh();
			}
		}
	}
	curs_set(1);
	refresh();
}

static void print_letter(t_letter letter)
{
	int color = 0;
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
