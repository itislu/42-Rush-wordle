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
static void print_welcome_art(void);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	if (!check_arguments(argc, argv))
	{
		return 1;
	}

	init_ncurses();
	t_word word = {0};
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char input[WORD_LENGTH + 1] = {'\0'};
	int guess_count = 0;

	print_welcome_art();
	pick_word(&word);
	if (g_mode & EVAL)
	{
		mvprintw(ROW_OFFSET - 2, INDENTATION, MSG_WORD_REVEAL, word.word);
	}
	print_grid();
	while (!is_game_finished(guesses, guess_count, &word))
	{
		if (!prompt_input(input, guess_count))
		{
			mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, INDENTATION, MSG_QUIT);
			break;
		}
		move(MAX_GUESSES + 1 + ROW_OFFSET, INDENTATION);
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

static void print_welcome_art(void)
{
	mvprintw(0, INDENTATION, WELCOME_ART1);
	mvprintw(1, INDENTATION, WELCOME_ART2);
	mvprintw(2, INDENTATION, WELCOME_ART3);
	mvprintw(3, INDENTATION, WELCOME_ART4);
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
		mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, INDENTATION, MSG_OUT_OF_GUESSES);
		mvprintw(MAX_GUESSES + 2 + ROW_OFFSET, INDENTATION, MSG_WORD_REVEAL, word->word);
		return true;
	}
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guesses[guess_count - 1][i].color != GREEN)
		{
			return false;
		}
	}
	mvprintw(MAX_GUESSES + 1 + ROW_OFFSET, INDENTATION, MSG_CORRECT, word->word);
	mvprintw(MAX_GUESSES + 2 + ROW_OFFSET, INDENTATION, MSG_GUESSES_NEEDED, guess_count);
	return true;
}
