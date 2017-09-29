#ifndef SAVELOAD_H_
#define SAVELOAD_H_

#include "Card.h";



//Structure for the file linked list
struct file_list {
	//File name
	char name[100];
	//Index of the file in the folder
	int index;
	//Next file
	struct file_list* next;
};

//Function used to open a file for saving
FILE* openNewFileForWriting(char[], struct file_list**);
//Function used to write the cards into the file
void writeCards(FILE* ptr, card* c, int sizeOfCards);
//Function used to write the drawn numbers into the file
void writeDrawnNumbers(FILE* ptr, int* numbersDrawn);
//Function used to save the game
struct file_list* saveGame(card* c, int sizeOfCards, int* numbersDrawn, char[], struct file_list**);


//Function used to scan the saved games dir
struct file_list* openSavedGamesDir();
//Function used to open a file for reading
FILE* openFileForReading(char[]);
//Function used to get the drawn numbers
void readDrawnNumbers(FILE*,int*);
//Function used to get the number of cards cards
int readCardNumber(FILE*);
//Function used to get the number of cards cards
void readCards(FILE* ptr, card* cards, int numberOfCards);

//Function used to get the last of items in file_list
int gteLastIndexOfFileList(struct file_list*);
//Function used to get file_list item by index
struct file_list* getFileListItemByIndex(struct file_list*, int);
#endif 
