#include <stdio.h>
#include <stdbool.h>
#include "Config.h"
#include "Card.h"
#include "SaveLoad.h"

//Function used to get the number of cards
int getNumberOfCards() {
	//The number
	int n;
	//Loop flag
	bool ok = false;
	//Do a loop for asking the number and checking the number
	while (!ok) {
		printf("\nPlease enter the number of cards between %d and %d:", MINIMUMCARDS, MAXIMUMCARDS);
		scanf("%d", &n);
		//Check if the number is between the allowed ones. If it is then set to true and exit the loop
		if (n >= MINIMUMCARDS && n <= MAXIMUMCARDS) ok = true;
	}
	//Return the number
	return n;
}

//Function print card
void printCard(card c) {
	//Iterator indexes
	int i, j;
	//Print empty line
	printf("\n");
	//Loop the rows
	for (i = 0; i < ROWSONCARD; i++) {
		printf("\n");
		//Loop the collumns
		for (j = 0; j < COLLUMNONCARD; j++) 
			//Write out the number
			printf(" %d", c.rows[i][j]);
		
	}
	//Print empty line
	printf("\n");
}
//Function used to print the drawn numbers
void printDrawnNumbers(int* drawnedNumbers) {
	//Iterator index
	int i,j,tmp;
	//Array for the numbers
	int numbers[MAXIMUMDRAWABLENUMBERS] = {0};
	//Check if there is any number is drawn
	if (*drawnedNumbers == 0) {
		printf("\n No number was drawn yet\n");
		return;
	}
	//Print empty line
	printf("\n\n Numbers drawn:\n");
	//Loop the numbers to print them
	for (i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) {
		//Check if the index is empty. If it is empty then exit the loop
		if (*(drawnedNumbers + i) == 0) break;
		//Print the number out
		printf(" %d", *(drawnedNumbers + i));
		//Add the number to the array
		numbers[i] = *(drawnedNumbers + i);
	}

	printf("\n\n Numbers drawn in order:\n");
	//Loop the numbers
	for (i = 0; i < MAXIMUMDRAWABLENUMBERS; ++i){
		//Loop the numbers with 1 shift
		for (j = i + 1; j < MAXIMUMDRAWABLENUMBERS; ++j){
			//If the previous number is higher than the current one and noe of them is 0
			if (numbers[i]  > numbers[j] && numbers[j]!=0 && numbers[i]!=0){
				//Then keep it in a temporary var
				tmp = numbers[i];
				//Then replace it with the smaller number
				numbers[i] = numbers[j];
				//Then take the temporary number into the smaller numbers place
				numbers[j] = tmp;
			}
		}
	}
	//Loop the ordered array
	for (i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) {
		//Check if the index is empty. If it is empty then exit the loop
		if (numbers[i] == 0) break;
		//Print the number out
		printf(" %d", numbers[i]);
	}
	//Print empty line
	printf("\n");
}

//Show the welcome message
void sayHi() {
	printf("\nWelcome to the Galway Bingo Ltd");
}

//Function choose menu option
int showMenuOptionChooser(bool loadedGame) {
	//Number of option
	int option;
	//Return option for if it is loaded game
	if (loadedGame) return 4;

	//Loop until an option is chosen
	do {
		printf("\n\nWhat would you like to do?");
		printf("\n1. Start new game");
		printf("\n2. Load game");
		printf("\n3. Exit");
		printf("\nEnter the number of the option:");
		scanf("%d", &option);
	} while (option < 1 && option > 3);
	//Return the option
	return option;
}

//Function used to show the menu for drawing the number or exit or save
int showGameOptionChooser(bool noMoreDraws) {
	//Number of option
	int option;

	//Loop until an option is chosen
	do {
		printf("\n\nWhat would you like to do?");
		if(!noMoreDraws) printf("\n1. Draw the next number");
		printf("\n2. Save game");
		printf("\n3. Output the games status");
		printf("\n4. Exit Without Save");
		printf("\nEnter the number of the option:");
		scanf("%d", &option);
	} while (option < 1 && option > 4);
	//Forward the menu to number 3 if 1 was chosen when it is not awailable
	if (noMoreDraws && option == 1) option = 3;

	//Return the option
	return option;
}

//Function used to show the save over write option
int showSaveOptionChooser(char fileName[]) {
	//Number of option
	int option;

	//Loop until an option is chosen
	do {
		printf("\n\nWould you like to over write the existing file?");
		printf("\nFile: %s",fileName);
		printf("\n1. Yes");
		printf("\n2. No");
		printf("\n3. Don't Save");
		printf("\nEnter the number of the option:");
		scanf("%d", &option);
	} while (option < 1 && option > 3);

	//Return the option
	return option;
}

//Function used to show file option chooser
int showFileOptionChooser(struct file_list* filelist) {
	//Number of option
	int option;
	//The last iindex in the list
	int lastFile = gteLastIndexOfFileList(filelist);
	//Loop until an option is chosen
	do {
		printf("\n\nWhich game would you like to load?");
		//Loop the list
		while (filelist != NULL) {
			//Write out the option
			printf("\n%d. %s",filelist->index, filelist->name);
			//Go to the next item
			filelist = filelist->next;
		}
		//Write out the exit option
		printf("\n%d. Go Back", lastFile+1);
		//Get the user to choose
		printf("\nEnter the number of the option:");
		scanf("%d", &option);
	} while (option < 0 && option > lastFile + 2);
	//If the option is to go back then set the option to -1;
	if (option == lastFile + 1) option = -1;

	//Return the option
	return option;
}

//Function used to show statistics of a card
void showCardStatistics(card c, int* numbersDrawn) {
	//Get the numbers needed for the corners rule to win
	int cornersRule = checkCornersRuleRemainingNumbers(c, numbersDrawn);
	//Get the numbers needed for the first line rule to win
	int firstLineRule = checkLineRuleRemaining(c,numbersDrawn);
	//Get the numbers needed for the second line rule to win
	int secondLineRule = checkTwoLineRuleRemaining(c, numbersDrawn);
	//Get the numbers needed for the full house rule to win
	int firstFullHouseRule = checkFullHouseRuleRemaining(c, numbersDrawn);
	
	//Print out the result
	//Corners rule
	if (cornersRule = -1) {
		printf("\nNumbers needed to Four corners: N/A");
	}else if (!cornersRule){
		printf("\nNumbers needed to Four corners: WON");
	} else {
		printf("\nNumbers needed to Four corners: %d", cornersRule);
	}

	//Line rule
	if (!firstLineRule) {
		printf("\nNumbers needed to Line: WON");
	} else {
		printf("\nNumbers needed to Line: %d", firstLineRule);
	}

	//Two line rule
	if (!secondLineRule) {
		printf("\nNumbers needed to Two Line: WON");
	} else {
		printf("\nNumbers needed to Two Line: %d", secondLineRule);
	}

	//Full House rule
	if (!firstFullHouseRule) {
		printf("\nNumbers needed to Full House: WON");
	} else {
		printf("\nNumbers needed to Full House: %d", firstFullHouseRule);
	}
}

//Function used to show statistics of every card
void showAllCardsStatistics(card* cptr,int numberOfPlayers, int* numbersDrawn) {
	//Iterator index
	int i;
	//Print empty line
	printf("\n");
	//Loop the crads
	for (i = 0; i < numberOfPlayers; i++) {
		printf("\n\nStatistics for Player %d:", i + 1);
		printCard(*(cptr + i));

		//Print the card statistics
		showCardStatistics(*(cptr + i),numbersDrawn);
	}
}

//Functionused to show the end of the game
void showEndGame() {
	printf("\n\nThe game has ended with the following results:");
}

//Show saved message
void showSaved() {
	printf("\n\nThe game is saved\n");
}
//Show loaded message
void showLoaded() {
	printf("\n\nThe game is loaded\n");
}

//Show can't load file
void showCantLoadGame() {
	printf("\n\nThe game can not be loaded\n");
}