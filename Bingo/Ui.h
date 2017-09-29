#ifndef UI_H_
#define UI_H_

//Show the welcome message
void sayHi();
//Function used to get the number of cards
int getNumberOfCards();
//Function print card
void printCard(card c);
//Function used to print the rawn numbers
void printDrawnNumbers(int*);
//Function choose menu option
int showMenuOptionChooser(bool);
//Function used to show the menu for drawing the number or exit or save
int showGameOptionChooser(bool);
//Function used to show the save over write option
int showSaveOptionChooser(char[]);
//Function show file option chooser
int showFileOptionChooser(struct file_list*);
//Function used to show statistics of a card
void showCardStatistics(card c, int* numbersDrawn);
//Function used to show statistics of every card
void showAllCardsStatistics(card* cptr, int numberOfPlayers, int* numbersDrawn);
//Functionused to show the end of the game
void showEndGame();
//Show saved message
void showSaved();
//Show loaded message
void showLoaded();
//Show can't load file
void showCantLoadGame();
#endif 
