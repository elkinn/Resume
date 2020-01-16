/*			
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * Intro Paragraph
 * Going through prog5.c, I looked at the purpose of each function and implemented them to act in the way they were supposed to. Using
 * sscanf, I ensured that the seed was a single integer, and seeded rand() with it once it was confirmed as such. In start_game, I
 * pseudorandomly generate 4 integers as the solution, set the static guess number to 1 and the static solution variables to the generated
 * solutions. In make_guess, I use sscanf to both confirm that the guess was 4 integers in the correct format, and to store the guesses to local
 * variables within the function (to loop through arrays more efficiently). First, it checks for perfect matches and marks the guesses and 
 * solutions as paired (to ensure no duplicate matches), then does the same for misplaced matches. Perfect matches being marked as paired before
 * this portion runs prevents a perfect match from also being marked here, so there's no need for extra protection. This tells the user how many
 * perfect and misplaced solutions they have, and increments the guess number. 
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int
set_seed (const char seed_str[])
{
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
//    int seed;
//    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below
    int seed;
    char post[2];
    if(sscanf (seed_str, "%d%1s", &seed, post) != 1){ //if post is defined, seed is invalid and 0 is returned
        printf("set_seed: invalid seed\n");
        return 0;
    }
    else{ //else, seed is valid and srand is called. 1 is returned
        srand(seed);
        return 1;
    }
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
    //fill memory at locations pointed to by one, two, three, and four with a random number 1-8
    *one = (rand() % 8) + 1;
    *two = (rand() % 8) + 1;
    *three = (rand() % 8) + 1;
    *four = (rand() % 8) + 1;
    guess_number = 1; //set guess_number to 1 at beginning of game
    solution1 = *one; //store the solutions to the static variables for use throughout various functions
    solution2 = *two;
    solution3 = *three;
    solution4 = *four;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two, 
	    int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.  
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed
    char post[2];
    if(sscanf(guess_str, "%d %d %d %d%1s", one, two, three, four, post) != 4 || *one > 8 || *one < 1 || *two > 8 || *two < 1
        || *three > 8 || *three < 1 || *four > 8 || *four < 1){ //if anything is wrong with the guess, its noticed here and the function returns 0
        printf("make_guess: invalid guess\n");
        return 0;
    }
    else{ //if guess is valid, solutions[] and guesses[] are created for ease of looping
        int solutions[4] = {solution1, solution2, solution3, solution4};
        int guesses[4] = {*one, *two, *three, *four};
        int perfect = 0; //placeholders for perfect and misplaced guesses created
        int imperfect = 0;
        int guesses_paired[4] = {0,0,0,0}; //contents of guesses_paired[] and sols_paired[] correspond to guesses and solutions and mark
        int sols_paired[4] = {0,0,0,0};    //whether they're paired or not. ex: if guesses_paired[1] and sols_paired[1] both == 1,
                                           //guesses[1] and soltuions[1] are paired
        for(int i=0; i<4; i++){     //check for perfect matches, where index of guess and solution are the same
            if(guesses[i] == solutions[i]){ //if perfect match, increment perfect matches and mark guess and solution as paired
                perfect++;
                guesses_paired[i] = 1;
                sols_paired[i] = 1;
            }
        }

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                if(guesses[i] == solutions[j] && !guesses_paired[i] && !sols_paired[j]){ //loop through all solutions for each guess, if guess
                    imperfect++;                                                         //matches solution and neither are paired, increment
                    guesses_paired[i] = 1;                                               //imperfect and mark them as paired.
                    sols_paired[j] = 1;
                }
            }
        }
        //print number of perfect and misplaced matches, increment guess_number, and return 1
        printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, perfect, imperfect);
        guess_number++;
        return 1;
    }
}


