#include "Rule.h"
#include "Card.h"
#include "Config.h"


//Function used to get the numbers needed to win the four corners rule
int checkCornersRuleRemainingNumbers(card c, int* numbersDrawn) {
	//The maximum row and collumn number
	int colmax = COLLUMNONCARD - 1;
	int rowmax = ROWSONCARD - 1;
	//The numbers got
	int numbersGot = 0;
	//Check if any of the four corner is missing from the card
	if (c.rows[0][0] == 0 || c.rows[0][colmax] == 0 || c.rows[rowmax][0] == 0 || c.rows[rowmax][colmax] == 0) return -1;

	//Check if all foru corner is drawn
	if (contains(numbersDrawn, MAXIMUMDRAWABLENUMBERS, c.rows[0][0]))           numbersGot++;
	if (contains(numbersDrawn, MAXIMUMDRAWABLENUMBERS, c.rows[0][colmax]))      numbersGot++;
	if (contains(numbersDrawn, MAXIMUMDRAWABLENUMBERS, c.rows[rowmax][0]))      numbersGot++;
	if (contains(numbersDrawn, MAXIMUMDRAWABLENUMBERS, c.rows[rowmax][colmax])) numbersGot++;
	//If the function hasnt return yet then the card did not win
	return 4 - numbersGot;
}

//Check four corner Rule
bool checkCornersRule(card c, int* numbersDrawn) {
	//If the function hasnt return yet then the card did not win
	return checkCornersRuleRemainingNumbers(c, numbersDrawn) == 0 ? true : false;
};


//Function used to get the number of drawn numbers in a line
int checkNumberOfDrawnNumbersInLine(card c, int line, int* numbersDrawn) {
	//Variable for the number of drawn numbers
	int numberOfDrawnNumbers = 0;
	//Iterator index
	int i;
	//Loop the card line
	for (i = 0; i < COLLUMNONCARD; i++)
		//If the number is not 0 and it is in the drawn numbers then increment the number of drawn numbers
		if (c.rows[line][i] != 0 && contains(numbersDrawn, MAXIMUMDRAWABLENUMBERS, c.rows[line][i])) numberOfDrawnNumbers++;
	//Return the numbers needed to finish
	return numberOfDrawnNumbers;
}
/*
//Function used to check how many numbers needed to finish a line
int checkLineRuleComplete(card c, int* numbersDrawn, int type) {
	//Function used to check a card if there is a two line win on it
	bool checkWin(card c, int* numbersDrawn, int type) {
		//Iterator index and lines win counter
		int i, won = 0;
		//Loopt the rows
		for (i = 0; i < ROWSONCARD; i++)
			//Increase the win counter if the line is fully drawn
			if (COLLUMNONCARD - SPACESINROWS - checkNumberOfDrawnNumbersInLine(c, line, numbersDrawn==0) {
				won++;
				//Return true if it is  one line check
				if (type == 1) return true;
				//Return true if it is two line check and two lines won
				if (type == 2 && won == 2) return true;
			}

		//Return true if it is full house check and all lines won
		if (type == 3 && won == ROWSONCARD) return true;

		//Return false if the function os not returned true yet
		return  false;
	}
	
	
	
	//Return the numbers needed to finish
	return COLLUMNONCARD - SPACESINROWS - checkNumberOfDrawnNumbersInLine(c, line, numbersDrawn);
}*/

//Function used to get the remainig numbers to win one line
int checkLineRuleRemaining(card c, int* numbersDrawn) {
	//Iterator index and lines win counter
	int i, needs = 5, tmp = 0;
	//Loopt the rows
	for (i = 0; i < ROWSONCARD; i++){
		//Get the remainig for the line
		tmp = COLLUMNONCARD - SPACESINROWS - checkNumberOfDrawnNumbersInLine(c, i, numbersDrawn);
		//The result is less then the current one then override the current one	
		if (tmp < needs) needs = tmp;
		//Return 0 if the line is fully drawn
		if (needs == 0) return 0;
	}
	//Return the lowest what needs to be drawn  
	return needs;
}

//Function used to get the remaining numbers to win two line
int checkTwoLineRuleRemaining(card c, int* numbersDrawn) {
	//Iterator index and lines win counter
	int i, j, remainings[ROWSONCARD], tmp = 0;
	//Loopt the rows
	for (i = 0; i < ROWSONCARD; i++) {
		//Get the remainig for the line
		remainings[i] = COLLUMNONCARD - SPACESINROWS - checkNumberOfDrawnNumbersInLine(c, i, numbersDrawn);
	}
	//Loop the numbers
	for (i = 0; i < ROWSONCARD; ++i) {
		//Loop the numbers with 1 shift
		for (j = i + 1; j < ROWSONCARD; ++j) {
			//If the previous number is higher than the current one and noe of them is 0
			if (remainings[i]  > remainings[j]) {
				//Then keep it in a temporary var
				tmp = remainings[i];
				//Then replace it with the smaller number
				remainings[i] = remainings[j];
				//Then take the temporary number into the smaller numbers place
				remainings[j] = tmp;
			}
		}
	}
	//Return the sum of the wo lowest 
	return remainings[0]+remainings[1];
}

//Function used to get the remainig numbers to win full house
int checkFullHouseRuleRemaining(card c, int* numbersDrawn) {
	//Iterator index and lines win counter
	int i, needs = 0;
	//Loopt the rows
	for (i = 0; i < ROWSONCARD; i++) {
		//Get the remainig for the line
		needs += COLLUMNONCARD - SPACESINROWS - checkNumberOfDrawnNumbersInLine(c, i, numbersDrawn);
	}
	//Return the lowest what needs to be drawn  
	return needs;
}

//Check line rule
bool checkLineRule(card c, int line, int* numbersDrawn) {
	//Check if all the numbers were drawn in the line
	return checkNumberOfDrawnNumbersInLine(c, line, numbersDrawn) == COLLUMNONCARD - SPACESINROWS;
};


//Function used to check a card if there is a two line win on it
bool checkWin(card c, int* numbersDrawn, int type) {
	//Iterator index and lines win counter
	int i, won = 0;
	//Loopt the rows
	for (i = 0; i < ROWSONCARD; i++)
		//Increase the win counter if the line is fully drawn
		if (checkLineRule(c, i, numbersDrawn)) {
			won++;
			//Return true if it is  one line check
			if (type == 1) return true;
			//Return true if it is two line check and two lines won
			if (type == 2 && won == 2) return true;
		}

	//Return true if it is full house check and all lines won
	if (type == 3 && won == ROWSONCARD) return true;

	//Return false if the function os not returned true yet
	return  false;
}


//Modify card winning state
card modifyCardWinningState(card c, int* numbersDrawn) {
	//Check and save the four corner win
	c.fourCornerWin = checkCornersRule(c, numbersDrawn);
	//Check and save the  line win
	c.lineWin = checkWin(c, numbersDrawn, 1);
	//Check and save the two line win
	c.twoLineWin = checkWin(c, numbersDrawn, 2);
	//Check and save the full house win(first and second and third line is Drawn)
	c.fullHouseWin = checkWin(c, numbersDrawn, 3);

	//Set the changed now flags
	if (c.fourCornerWin && !c.fourCornerWinChangedNow) c.fourCornerWinChangedNow = true;
	if (c.lineWin && !c.lineWinChangedNow) c.lineWinChangedNow = true;
	if (c.twoLineWin && !c.twoLineWinChangedNow) c.twoLineWinChangedNow = true;
	if (c.fullHouseWin  && !c.fullHouseWinChangedNow ) c.fullHouseWinChangedNow  = true;


	//Return the card back
	return c;
}

