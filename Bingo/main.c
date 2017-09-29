#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "Card.h"
#include "Config.h"
#include "Ui.h"
#include "Rule.h"
#include "Draw.h"
#include "SaveLoad.h"



void main(){
	//Pointer for the Drawn numbers
	int* numbersDrawn = (int*)malloc(MAXIMUMDRAWABLENUMBERS  * sizeof(int));
	//Number of cards in the game
	int numberOfCards=0;
	//Cards
	card* cards;
	//Iterator indexes
	int ci;
	//Flag for running the game(draw)
	bool rungame = true;
	//No more draws
	bool noMoreDraws=false;
	//Flag to keep the app runing
	bool runApplication = true;
	//Flag to indicate full house
	bool fullHouseReached = false;
	//Opened file pointer
	FILE* openedFilePointer;
	//Flag for loadedgame
	bool loadedGame = false;
	//File load option
	short fileLoadOption;
	//Loaded items from the folder
	struct file_list* fileList=NULL;
	//File for loading
	struct file_list** fileForLoading = (struct file_list**)malloc(sizeof(struct file_list*));
	//Flag to indicate file over write
	bool overWriteFIle = true;
	//File name to save
	char saveGameFileName[36] ="-1";
	//Initialize the drawn numbers to 0;
	for (int i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) numbersDrawn[i] = 0;

	//Initialize random number generator
	srand((unsigned)time(NULL));
	
	//Show welcome message
	sayHi();

	

	//Keep running the game
	while (runApplication) {
		//Show the menu and chose operation
		switch (showMenuOptionChooser(loadedGame)) {
			case 1:
				//Reset file for loading
				//It is needed if the user already opened a game then created a new and and wants to save after
				*fileForLoading = NULL;

				//Initialize the pulled numbers to 0;
				for (int i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) numbersDrawn[i] = 0;
				
				//No more draws
				bool noMoreDraws = false;
				//Flag to indicate full house
				bool fullHouseReached = false;

				//Get the number of cards from the user
				numberOfCards = getNumberOfCards();

				//Allocate the  memory for the cards
				cards = (card*)malloc(numberOfCards * sizeof(card));

				//Loop to create the cards
				for (ci = 0; ci < numberOfCards; ci++) {
					//Create the cards
					cards[ci] = createCard(false,ci);

					//printCard(*(cards + ci));
				}

				//Set the flag to loaded game
				loadedGame = true;
				//Set the flag to run game
				rungame = true;

				break;
			case 2:
				//Load the saved games from the dir
				fileList=openSavedGamesDir();
				//Show the option chooser to the user
				fileLoadOption=showFileOptionChooser(fileList);
				//Go back to the menu if the option is -1;
				if (fileLoadOption == -1) {
					//Set the flag for loaded game
					loadedGame = false;
					//Break the switch
					break;
				}

				//Get the list item
				fileForLoading[0]=getFileListItemByIndex(fileList, fileLoadOption);
				//If the item exists
				if (*fileForLoading != NULL) {
					//Open the file
					openedFilePointer = openFileForReading((*fileForLoading)->name);
					//Load the drawn numbers
					readDrawnNumbers(openedFilePointer, numbersDrawn);
					//Get the number of cards
					numberOfCards = readCardNumber(openedFilePointer);
					//Allocate the  memory for the cards
					cards = (card*)malloc(numberOfCards * sizeof(card));
					//Loop to create empty cards cards
					for (ci = 0; ci < numberOfCards; ci++) {
						//Create the cards
						cards[ci] = createCard(true,ci);
					}
					//Read the cards from the file
					readCards(openedFilePointer, cards, numberOfCards);

					//Set the flags to default before checking the cards
					fullHouseReached = false;
					noMoreDraws = false;

					//Loop to check the cards if any of them won with the now card
					for (ci = 0; ci < numberOfCards; ci++) {
						//Set the winning states for the cards
						cards[ci] = modifyCardWinningState(*(cards + ci), numbersDrawn);
						//Check if there is any full house yet
						if (cards[ci].fullHouseWin) {
							fullHouseReached = true;
							noMoreDraws = true;
						}

					}
					//Show loaded message
					showLoaded();
					//Set the flag for loaded game
					loadedGame = true;
					//Set the flag to run the game
					rungame = true;

					//Display current status of the game
					//Print game ended message if there is no more draws
					if (!noMoreDraws) showEndGame();
					//Print the numbers drawn
					printDrawnNumbers(numbersDrawn);
					//Print the player statistics
					showAllCardsStatistics(cards, numberOfCards, numbersDrawn);


					//Break the switch
					break;
				} else {
					//Show can't load file
					showCantLoadGame();
					//Break the switch
					break;
				}
				
				
			case 3:
				//Exit application
				runApplication = false;
				break;
				//The game loop
			case 4:
				//Loop until the user choses other option than draw the next 
				while (rungame == true){
					//Get the option from the user
					switch (showGameOptionChooser(fullHouseReached)) {
						//If draw option
					case 1:
						//Draw a new number
						noMoreDraws = drawNumber(numbersDrawn);
						//Loop to check the cards if any of them won with the now card
						for (ci = 0; ci < numberOfCards; ci++) {
							//Set the winning states for the cards
							cards[ci] = modifyCardWinningState(*(cards + ci), numbersDrawn);
							//Check if there is any full house yet
							if (cards[ci].fullHouseWin) fullHouseReached = true;
						}
						break;
						//Save the game
					case 2:

						//Check if the game is reviously opened
						if (*fileForLoading != NULL) {
							//Show the file chooser
							switch(showSaveOptionChooser((*fileForLoading)->name)) {
									//Yes
								case 1:
									//Copy the file name
									strcpy(saveGameFileName, (*fileForLoading)->name);
									break;
									//No
								case 2:
									//Set the file name to be empty
									strcpy(saveGameFileName, "");
									break;
									//Go back
								case 3:
									//Set the file name to -1
									strcpy(saveGameFileName, "-1");
									break;
							}
						} else {
							//Set the file name to be empty
							strcpy(saveGameFileName, "");
						}
						
						//Check if the game should be saved
						if (strcmp(saveGameFileName, "-1") != 0) {
							//Save the game
							fileForLoading[0]=saveGame(cards, numberOfCards, numbersDrawn, saveGameFileName, fileForLoading);
							//Show saved message
							showSaved();
						}
						break;
						//Print the status of the game
					case 3:
						//Print game ended message if there is no more draws
						if (!noMoreDraws) showEndGame();
						//Print the numbers drawn
						printDrawnNumbers(numbersDrawn);
						//Print the player statistics
						showAllCardsStatistics(cards, numberOfCards, numbersDrawn);
						break;
						//If exit without save
					case 4:
						//Exit the loop
						rungame = false;
						//Set the flag for loaded game
						loadedGame = false;
						break;
					}

				}
				break;

		}
	}


	

	



	

}