#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MAXLENGTH 7
#define TOTALGUESSES 20
#define toUppercase(c) if (c >= 'a' && c <= 'z') \
                            { \
                                c+= ('A' - 'a'); \
                            }

int bgetline(char s[], int putNl);
int comparePattern(char input[], char pattern[]);

int main(){
    char pattern[MAXLENGTH], input[MAXLENGTH];
    int randomNum = 0, nl;
    int i, iter; //Counter vars
    int inputLen = 0; //Length of input line
    int playAgain = 1; //Whether the user wants to keep playing; starts as true
    int userCorrect = 0; //User's guess correct defaults to false

    srand(time(NULL));   // Initialization, should only be called once.

    while (playAgain)
    {
        //Create the pattern
        for (i = 0; i < 5; i++){
            /* random int between 0 and 6 */
            randomNum = rand() % 7;
            pattern[i] = 'A' + randomNum;
        }
        pattern[i] = '\0';

        //Welcome user and get their first guess
        printf("MASTERMIND GAME\n\n");
        printf("Enter H for help at any time.\n");

        //Reset the userCorrect value
        userCorrect = FALSE;
        //Gets the guesses and sends them to be compared
        for (i = 0; (i < TOTALGUESSES) && (userCorrect != TRUE); i++){
            printf("You have %i guesses left.\n", TOTALGUESSES - i);
            printf("Please enter a sequence of five letters from A to G:\n");
            inputLen = bgetline(input, 0);
    //        printf("DEBUG 3: inputLen: %i\n", inputLen);
            if (inputLen == 1)
            {
                printf("Mastermind is a game of deduction and logic. The computer generates a pattern of five letters selected from A to G. Your goal is to guess this pattern using clues provided by the computer after each guess. You have %i guesses. When one of your letters is in the correct place, the computer says that one peg dropped into place. When one of the letters is in the sequence but not in the correct spot, the computer says that one peg stuck in the hole.\n", TOTALGUESSES);
                i--; //So don't use a guess
                continue;
            }
            else if (inputLen > 5)
            {
                printf("You entered too many letters. Please try again.\n");
                i--;
                nl = 0; //In case the loop has already run and nl has been used already
                for (iter = 0; iter < MAXLENGTH && (nl != '\n'); iter++){
                    nl = getchar();
                }
                if (iter == MAXLENGTH)
                {
                    printf("ERROR: Too many characters! Cannot absorb all of them. Game over.\n");
                    return 1;
                }
                continue;
            }
            //inputLen == 1 has already been checked, so no need to do so again
            else if (inputLen < 5)
            {
                printf("You did not enter enough letters. Please try again.\n");
                i--;
                continue;
            }
            else ; //Required, but nothing needed for it to do

            userCorrect = comparePattern(input, pattern);

            if (userCorrect)
            {
                //Good job for winning
                printf("You won using only %i guesses.\n", i);
                break;
            }
        }

        //If they ran out of guesses
        if (i == TOTALGUESSES)
        {
            printf("You ran out of guesses. The pattern was %s. Better luck next time!\n", pattern);
        }

        //Play again?
        printf("Play again? (y/n): \n");
        playAgain = getchar();
        nl = getchar();
        toUppercase(playAgain);
        if (playAgain == 'Y')
        {
            playAgain = TRUE;
        }
        else playAgain = FALSE;
    }

    return 0;
} //end of main

//Get a line of input from the keyboard
int bgetline(char s[], int putNl){
	int i, c;

	for (i=0; i<MAXLENGTH-1 && (c=getchar()) !='$' && c!='\n'; ++i){
		s[i] = c;
    //    printf("DEBUG 1: c: %c\n", c);
    }
    //printf("DEBUG 2: testing\n");
	if ((c == '\n') && (putNl == 1))
	{
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
} //end of bgetline

//Compare a pattern to input
int comparePattern(char input[], char pattern[]){
    int i, k, match, inPattern, matchIndex[MAXLENGTH], inPatternIndex[MAXLENGTH];
    match = inPattern = 0;

    //printf("DEBUG 4: pattern: %s\n", pattern);

    //See if any letters are in the right place
    for (i = 0; pattern[i] != '\0' && i < MAXLENGTH; i++){
        toUppercase(input[i]);
        if (input[i] == pattern[i])
        {
            match++;
            matchIndex[i] = TRUE; //To prevent the inPattern search from duplicating matches
        //    printf("DEBUG 5: i: %i\ninput[i]: %c\n", i, input[i]);
        }
        else matchIndex[i] = FALSE;

        inPatternIndex[i] = FALSE; //Filling this here
    }
    if (match == 5)
    {
        return 1; //Then the pattern matched exactly
    }
    //See if any letters are contained in the pattern but aren't in the right places
    for (i = 0; pattern[i] != '\0' && i < MAXLENGTH; i++){
        for (k = 0; input[k] != '\0' && k < MAXLENGTH; k++){
        //    printf("DEBUG 6: inPatternIndex[k]: %i\n", inPatternIndex[k]);
            if ((input[k] == pattern[i]) && (matchIndex[i] == FALSE) && (inPatternIndex[k] == FALSE) && (matchIndex[k] == FALSE))
            {
                inPattern++;
                //printf("DEBUG 7: k: %i\ni: %i\ninput[k]: %c\npattern[i]: %c\ninPatternIndex[k]: %i\n", k, i, input[k], pattern[i], inPatternIndex[k]);
                //printf("DEBUG 8: matchIndex[i]: %i\nmatchIndex[k]: %i\n", matchIndex[i], matchIndex[k]);
                inPatternIndex[k] = TRUE; //Says that this letter in input has been matched already
                //printf("DEBUG 9: input[k] %c == pattern[i] %c\n", input[k], pattern[i]);
                break;
            }
        }
    }

    //Print clues
    printf("You see %i %s drop into place.\n", match, ((match != 1) ? "pegs" : "peg"));
    printf("You also see %i %s stuck in %s.\n", inPattern, ((inPattern != 1) ? "pegs" : "peg"), inPattern != 1 ? "holes" : "a hole");

    return 0;
} //end of comparePattern
