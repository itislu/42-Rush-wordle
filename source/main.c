#include "wordle.h"
#include <locale.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <unistd.h>

t_mode g_mode = STANDARD;

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], 
	int guess_count, 
	const t_word *word);
static void init_colors();

short rgb_to_ncurses(int rgb)
{
	return rgb * 1000 / 255;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	if (!check_arguments(argc, argv))
	{
		return 1;
	}

	initscr();
	init_colors();
	cbreak();
	t_word word = {0};
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char input[WORD_LENGTH + 1] = {'\0'};
	int guess_count = 0;

	printw(WELCOME_ART);
	pick_word(&word);
	if (g_mode & EVAL)
	{
		mvprintw(ROW_OFFSET - 2, 0, MSG_WORD_REVEAL, word.word);
	}
	print_grid();
	while (!is_game_finished(guesses, guess_count, &word))
	{
		if (!prompt_input(input, guess_count))
		{
			mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, 0, MSG_QUIT);
			break;
		}
		move(MAX_GUESSES + 1 + ROW_OFFSET, 0);
		clrtoeol();
		if (!validate_input(input))
		{
			continue;
		}
		convert_input(input, guesses[guess_count]);
		compare_guess(guesses[guess_count], word);
		guess_count++;
	}
	curs_set(0);
	refresh();
	getch();
	free(word.word);
	endwin();
}

static void init_colors()
{
	start_color();

	init_color(COLOR_GREEN, 
		rgb_to_ncurses(83), rgb_to_ncurses(141), rgb_to_ncurses(78));
	init_color(COLOR_YELLOW, 
		rgb_to_ncurses(180), rgb_to_ncurses(164), rgb_to_ncurses(60));
	init_color(COLOR_GREY, 
		rgb_to_ncurses(58), rgb_to_ncurses(58), rgb_to_ncurses(60));

	init_pair(GREEN, COLOR_WHITE, COLOR_GREEN);
	init_pair(YELLOW, COLOR_WHITE, COLOR_YELLOW);
	init_pair(GREY, COLOR_WHITE, COLOR_GREY);
}

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], 
	int guess_count, 
	const t_word *word)
{
	print_guesses(guesses, guess_count);
	if (guess_count == 0)
	{
		return false;
	}
	if (guess_count >= MAX_GUESSES)
	{
		mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, 0, MSG_OUT_OF_GUESSES);
		mvprintw(MAX_GUESSES + 2 + ROW_OFFSET, 0, MSG_WORD_REVEAL, word->word);
		return true;
	}
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guesses[guess_count - 1][i].color != GREEN)
		{
			return false;
		}
	}
	mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, 0, MSG_CORRECT, word->word);
	mvprintw(MAX_GUESSES + 2 + ROW_OFFSET, 0, MSG_GUESSES_NEEDED, guess_count);
	return true;
}
