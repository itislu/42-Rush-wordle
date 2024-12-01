#include "wordle.h"
#include <ncursesw/curses.h>

static void init_colors(void);
static inline short rgb_to_ncurses(int rgb);

void init_ncurses(void)
{
	initscr();
	init_colors();
	cbreak();
}

static void init_colors(void)
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

static inline short rgb_to_ncurses(int rgb)
{
	return rgb * 1000 / 255;
}
