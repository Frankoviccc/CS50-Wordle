#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);

int input;
int main(int argc, string argv[])
{
    // ensure proper usage
    // TODO #1

    // Check if there is more than one argument that the user prompt
    // If yes, return. If not, turn string into integer
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }
    else
    {
        input = atoi(argv[1]);
    }

    // TODO #2

    // ensure argv[1] is either 5, 6, 7, or 8 and store that value in wordsize instead
    // If the user prompt a higher or shorter length than our game allows, explain.
    if (input < 5 || input > 8)
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n");
        return 1;
    }
    // Put length of input in wordsize
    int wordsize = input;



    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        // set all elements of status array initially to 0, aka WRONG
        // TODO #4
        for (int x = 0; x < wordsize; x++)
        {
            status[x] = WRONG;
        }
        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    // Print the game's result
    // TODO #7
    if (won)
    {
        printf("You won!\n");
    }
    else
    {
        printf("The correct word we was looking for is: %s\n", choice);
    }

    // that's all folks!
    return 0;
}

string get_guess(int wordsize)
{
    string guess = "";

    // ensure users actually provide a guess that is the correct length
    // TODO #3

    // Keep prompting user untill user gives a word with the correct length
    do
    {
        guess = get_string("Input a %d-letter word: ", wordsize);
    }
    while (strlen(guess) != wordsize);
    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;
    // compare guess to choice and score points as appropriate, storing points in status
    // TODO #5

    // Create for loop that iterates trough guessed and choice words
    for (int i = 0; i < strlen(guess); i++)
    {
        // if the index of guessed word matches with the index of secret word
        // Update score with +2 and update the value to his place in the status array
        if (guess[i] == choice[i])
        {
            score += EXACT;
            status[i] = EXACT;
        }
        else
        {
            // If the first loops indexes didnt match, loop again to look if there are equal letters on different places
            // Use the index of the first loop so you start with the letter where the first loop ended
            // Now use the indexes of both loops so you don't compare per letter but also different indexes
            for (int j = 0; j < strlen(guess); j++)
            {
                if (guess[i] == choice[j])
                {
                    status[i] = CLOSE;
                    score += CLOSE;
                }
            }
        }
    }
    // HINTS
    // iterate over each letter of the guess
    // iterate over each letter of the choice
    // compare the current guess letter to the current choice letter
    // if they're the same position in the word, score EXACT points (green) and break so you don't compare that letter further
    // if it's in the word, but not the right spot, score CLOSE point (yellow)
    // keep track of the total score by adding each individual letter's score from above

    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    // print word character-for-character with correct color coding, then reset terminal font to normal
    // TODO #6

    // Use the values of the status array to print out the colour that matches the score of the letter
    for (int i = 0; i < strlen(guess); i++)
    {
        if (status[i] == 1)
        {
            printf(YELLOW"%c", guess[i]);
        }
        else if (status[i] == 2)
        {
            printf(GREEN"%c", guess[i]);
        }
        else
        {
            printf(RED"%c", guess[i]);
        }
    }
    printf(RESET"\n");
    return;
}
