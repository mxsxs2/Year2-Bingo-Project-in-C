#ifndef CARD_H_
#define CARD_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
//Type definition for a card
typedef struct{
	int rows[3][9];
	bool fourCornerWin;
	bool lineWin;
	bool twoLineWin;
	bool fullHouseWin;
	bool fourCornerWinChangedNow;
	bool lineWinChangedNow;
	bool twoLineWinChangedNow;
	bool fullHouseWinChangedNow;
}card;

//Create card prototype
card createCard(bool,int);
//Get random number
int randomNumber(int*, int,int,int);
//Create four random spaces
int* randomSpaces();
//Function used to check if an int pointer array conatins a number
bool contains(int*, int, int);
//Fucntion insert into used numbers
void insertIntoPointerArray(int*, int, int);
#endif 