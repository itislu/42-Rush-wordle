#include "wordle.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t g_stop = 0;

static bool is_game_finished(
	t_letter guesses[MAX_GUESSES][WORD_LENGTH], 
	int guess_count, 
	const t_word *word);
static void init_colors();

short rgb_to_ncurses(int rgb)
{
	return rgb * 1000 / 255;
}

void handle_sigint(int signo)
{
	printf("received signal: %d", signo);
	if (signo == SIGINT)
	{
		g_stop = 1;
	}
}

int main(void)
{
	initscr();
	init_colors();
	cbreak();
	//signal(SIGINT, &handle_sigint);
	t_word word = {0};
	t_letter guesses[MAX_GUESSES][WORD_LENGTH] = {0};
	char input[WORD_LENGTH + 1] = {'\0'};
	int guess_count = 0;

	//printf("%s\n", WELCOME_ART);
	pick_word(&word);
	print_grid();
	// sleep(2);
	while (!is_game_finished(guesses, guess_count, &word))
	{
		if (!prompt_input(input, guess_count))
		{
			mvprintw(MAX_GUESSES + 1, 0, "Goodbye!");
			break;
		}
		move(MAX_GUESSES + 1, 0);
		clrtoeol();
		if (!validate_input(input))
		{
			continue;
		}
		convert_input(input, guesses[guess_count]);
		// print_current_row(guesses, guess_count);
		compare_guess(guesses[guess_count], word);
		print_guesses(guesses);
		guess_count++;
	}
	curs_set(0);
	refresh();
	sleep(5);
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
	if (guess_count == 0)
	{
		return false;
	}
	if (guess_count >= MAX_GUESSES)
	{
		mvprintw(MAX_GUESSES + 1, 0, "You ran out of guesses!");
		return true;
	}
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		if (guesses[guess_count - 1][i].color != GREEN)
		{
			return false;
		}
	}
	print_guesses(guesses);
	mvprintw(MAX_GUESSES + 1, 0, "You guessed '%s' correctly!", word->word);
	mvprintw(MAX_GUESSES + 2, 0, "Guesses needed: %d", guess_count);
	return true;
}
