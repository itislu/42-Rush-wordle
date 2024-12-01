#include "utils.h"
#include "wordle.h"
#include "words.h"
#include <ctype.h>
#include <ncursesw/curses.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static bool is_valid_length(const char *input);
static bool is_alphabetic(const char *input);
static bool is_in_dictionary(const char *input);

bool prompt_input(char *input, int guess_count)
{
	int i = 0;
	int c;
	memset(input, '\0', WORD_LENGTH);
	move(guess_count, i + i + GUESSES_INDENTATION);
	fill_row();
	noecho();
	keypad(stdscr, TRUE);
	while ((c = getch()) != EOF)
	{
		//printw("%d", c);
		if (c == '\n')
			break;
		if (c == KEY_LEFT || c == KEY_RIGHT || c == KEY_LEFT || c == KEY_RIGHT)
			continue;
		else if (c == KEY_BACKSPACE)
		{
			if (i == 0)
				continue;
			else if (i == WORD_LENGTH)
				curs_set(1);
			i -= 1;
			input[i] = '\0';
			mvprintw(guess_count, i + i + GUESSES_INDENTATION, "_");
			move(guess_count, i + i + GUESSES_INDENTATION);
			// delch();
			// delch();
			// delch();
			// delch();
		}
		else if (i < WORD_LENGTH && isalpha(c))
		{
			c = toupper(c);
			mvprintw(guess_count, i + i + GUESSES_INDENTATION, "%c", c);
			input[i] = c;
			i++;
			if (i == WORD_LENGTH)
				curs_set(0);
			move(guess_count, i + i + GUESSES_INDENTATION);
		}
		//else if (input[i])
		//while (i == 5 && != '\n')
	}
	curs_set(1);
	input[WORD_LENGTH] = '\0';
	str_toupper(input);
	return true;
}

bool validate_input(const char *input)
{
	if (!is_valid_length(input))
	{
		mvprintw(MAX_GUESSES + 1, 0, MSG_INVALID_LENGTH);
		return(false);
	}
	if (!is_alphabetic(input))
	{
		mvprintw(MAX_GUESSES + 1, 0, MSG_INVALID_CHARACTER);
		return(false);
	}
	if (!is_in_dictionary(input))
	{
		mvprintw(MAX_GUESSES + 1, 0, MSG_INVALID_WORD);
		return(false);
	}
	return (true);
}

static bool is_valid_length(const char *input)
{
	int i = strlen(input);
	if (i == WORD_LENGTH)
		return (true);
	return (false);
}
static bool is_alphabetic(const char *input)
{
	int i = 0;
	while (input[i])
	{
		if (!isalpha(input[i]))
			return (false);
		i++;
	}
	return (true);
}
static bool is_in_dictionary(const char *input)
{
	char word[WORD_LENGTH + 1] = {'\0'};
	int i = 0;
	while (g_dict[i])
	{
		memcpy(word, g_dict[i], WORD_LENGTH);
		str_toupper(word);
		if (strcmp(input, word) == 0)
			return (true);
		i++;
	}
	return (false);
}
