#include "wordle.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

bool check_arguments(int argc, char *argv[])
{
	(void)argc;
	for (int i = 1; argv[i] != NULL; i++)
	{
		if (strcmp(argv[i], "--eval") == 0)
		{
			g_mode |= EVAL;
		}
		else
		{
			dprintf(STDERR_FILENO, "Invalid argument: %s\n", argv[i]);
			return false;
		}
	}
	return true;
}
