# Logic

## Input checking
- [ ] Length
- [ ] Alphabetical
- [ ] In dictionary

If bad, prompt again

## Comparison
1. Green: Go through the input and check each character if correct
	- compare guess with target_word
	- 1 to 1 comparison between guess and target_word
2. Yellow: For each non-green character, check if it appears in the word
	- compare guess with target_word
	- for each not green char in guess: `strchr(target_word, input[i]) != NULL`
3. Grey: Check green characters if they appear as yellow anywhere in the word and overwrite those to grey
	- compare guess with itself

# Implementation

## Defines

- `WORD_LENGTH 5`
- `MAX_GUESSES 6`

## Variables

### Dictionary
In file:
- Read (error handling included) -> data structure tbd

For prototype:
- `# define`

### Guess counter

Needed in the main loop

### Guesses

- Array of Guesses, size 6
  Guess:
	- string (without `'\n'`)
	- array of Letters
	  Letter:
	  ```c
	  struct Letter {
	  	char letter;
	  	enum colors color;
	  }
	  ```

## Functions

- `void print_guesses()`
	- `void print_letter()`

- `char *prompt_input()`
	- `to_lower(char *input)`

- `bool validate_input(char *input)`: if not valid, prompt again
	- `bool is_valid_length()`
	- `bool is_alphabetic()`
	- `bool is_in_dictionary()`

- `Guess *convert_input(char *input)`: convert input to Guess

- `compare_guess(Guess *guess)`: set the colors in the guess

### Utils

- `bool is_game_finished()`



## Interface

- [ ] research ncurses


# Improvements

## Backend

- [ ] Add debug/evaluation mode
- [ ] More common words to guess as a default word list to guess from
- [ ] Larger default dictionary
- [ ] Option to read the dictionary from a file
- [ ] Option to read the word list to guess from a file
- [ ] Sanitize dictionary from a file for ASCII words only

## Frontend

- [ ] When running out of guesses, show the word
- [ ] Write input directly into rectangle of guesses
- [ ] Update the interface instead of printing anew after each guess
- [ ] Zoom-in the terminal at startup
- [ ] Display keyboard to see which letters already guessed
- [ ] Limit input reading to 5 characters and ASCII letters only
