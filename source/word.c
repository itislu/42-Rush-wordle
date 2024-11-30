#include "utils.h"
#include "wordle.h"
#include "words.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void count_occurances(t_word *word);

void pick_word(t_word *word)
{
	srandom(time(NULL));
	long idx = random() % (sizeof(g_dict) / sizeof(char *));
	word->word = strdup(g_dict[idx]);

	str_tolower(word->word);
	count_occurances(word);
}

static void count_occurances(t_word *word)
{
	for (int i = 0; word->word[i] != '\0'; i++)
	{
		word->alphabet[index_alpha(word->word[i])]++;
	}
}
