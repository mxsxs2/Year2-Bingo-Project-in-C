#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Card.h"
#include "Config.h"

//Function used to create a new card from numbers that are not used yet
card createCard(bool empty,int card_number) {
	//The card
	card c;
	//Iterator indexes
	int i,j,g;
	//Spaces holder
	int* spaces;
	//Default, maximum and minimum numbers 
	int max= LARGESTNUMBERONCARD, min=1;
	//Allready placed numbers
	int*notAllowedNumbers  = (int*)malloc(LARGESTNUMBERONCARD * sizeof(int));
	//Initialize placed numbers to 0;
	for (int i = 0; i < LARGESTNUMBERONCARD; i++) notAllowedNumbers[i] = 0;
	//Initialize card winning states
	c.fourCornerWin = c.lineWin = c.twoLineWin = c.fullHouseWin = false;
	c.fourCornerWinChangedNow = c.lineWinChangedNow = c.twoLineWinChangedNow = c.fullHouseWinChangedNow = false;

	//Check if the card should be empty
	if (!empty) {
		//Loop the rows
		for (i = 0; i < ROWSONCARD; i++) {
			//Generate spaces
			spaces = randomSpaces();
			//Loop the collumns
			for (j = 0; j < COLLUMNONCARD; j++) {
				//Check if the index is a space
				if (contains(spaces, SPACESINROWS, j)) {
					//Set the value as a space
					c.rows[i][j] = 0;
					//Go to the next index
					continue;
				}

				//Decide the minimum and the maximum range for the random generator.
				switch (j) {
					case 0:
						min = 1;
						max = 10;
						break;
					case 1:
						min = 11;
						max = 20;
						break;
					case 2:
						min = 21;
						max = 30;
						break;
					case 3:
						min = 31;
						max = 40;
						break;
					case 4:
						min = 41;
						max = 50;
						break;
					case 5:
						min = 51;
						max = 60;
						break;
					case 6:
						min = 61;
						max = 70;
						break;
					case 7:
						min = 71;
						max = 80;
						break;
					case 8:
						min = 81;
						max = 90;
						break;
				}


				//Create a random number;
				c.rows[i][j] = randomNumber(notAllowedNumbers, LARGESTNUMBERONCARD, max, min);
				//Insert the number into the used numbers
				insertIntoPointerArray(notAllowedNumbers, LARGESTNUMBERONCARD, c.rows[i][j]);
			}
			free(spaces);
		}
	} else {
		//Make the cards empty
	}


	//bool
	return c;
}


//Get random number
int randomNumber(int* notAllowedNumbers, int length, int max, int min) {
	//Random Number
	int rnd;
	//Flag for uniqueness
	int unique = 1;

	//Do while loop for getting the random number within the given range
	do {
		//Create random number
		rnd = rand() % max + 1;
	} while (rnd > max || rnd < min);
	
	
	if (contains(notAllowedNumbers, length, rnd)) unique = 0;
	
	
	//If the number is allowed then return it, if not then call the function again.
	return unique==1 ? rnd : randomNumber(notAllowedNumbers, length,max, min);
}
//Create four random spaces
int* randomSpaces() {
	//Allocate the memory for the array
	int* spaces = (int*)malloc(SPACESINROWS *sizeof(int));
	//Iterator index
	int i;

	//Initialize the spaces to 0
	for (i = 0; i < SPACESINROWS; i++) spaces[i] = 0;

	//Generate the spaces
	for (i = 0; i < SPACESINROWS; i++) spaces[i] = randomNumber(spaces, SPACESINROWS, COLLUMNONCARD-1,1);
	
	//Return the pointer
	return spaces;
}

//Function used to check if an int pointer array conatins a number
bool contains(int* pointer, int length, int number) {
	//Iterator index
	int i;
	int tmpnum = 0;
	//Loop the pointer array and check if the number is in it
	for (i = 0; i < length; i++) {
		//printf("\n i: %d, num: %d, rnd: %d", i, *(pointer + i), number);
		
		//printf("\n%s",sizeof());
		if (*(pointer+i) == number) return true;
	}
	//Number was not found, so return false
	return false;
}

//Fucntion insert into used numbers
void insertIntoPointerArray(int* pointer, int length, int number) {
	//Iterator index
	int i;
	//Loop the pointer array 
	for (i = 0; i < length; i++) {
		//If the current number is the same as the number to be added then break the loop. No duplicates allowed
		if (*(pointer + i) == number) break;

		//If the current number is 0
		if (*(pointer + i) == 0) {
			//Add the number
			pointer[i] = number;
			//Exit the loop
			break;
		}
	}
}

