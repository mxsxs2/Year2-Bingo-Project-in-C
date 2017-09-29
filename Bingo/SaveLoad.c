#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Card.h"
#include "Config.h"
#include "SaveLoad.h"
#include "tinydir.h" //https://github.com/cxong/tinydir
 


//Function used to open a file for saving
FILE* openNewFileForWriting(char fileName[], struct file_list** loadedFile) {
	//Time holder
	time_t rawtime;
	//File name  holder
	char buffer[36];
	//Get the time
	time_t now=time(NULL);
	//If new file should be created
	if (strcmp(fileName, "") == 0) {
		//Format the time and create the name of the file from it
		strftime(buffer, sizeof(buffer), "Bingo_save_%Y_%m_%d_%H_%M_%S.bng", gmtime(&now));
	}else {
		//Copy the file name to the buffer
		strcpy(buffer, fileName);
	}
	//Create an item
	loadedFile[0] = (struct file_list*)malloc(sizeof(struct file_list));
	//Set the attributes of the item
	strcpy((*loadedFile)->name, buffer);
	(*loadedFile)->index = 0;
	(*loadedFile)->next = NULL;
	
	//Set the name of the fileList

	//Open a new file with the current time in the name
	return fopen(buffer, "w");
};

//Function used to write the cards into the file
void writeCards(FILE* ptr, card* c, int sizeOfCards) {
	//Iterator index
	int ci,i,j;
	//Card tmp
	card tmp;
	//If the file is open
	if (ptr != NULL) {

		//Write the size of cards into the file
		fprintf(ptr, "%d\n", sizeOfCards);
		//Loop the cards
		for (ci = 0; ci < sizeOfCards; ci++) {
			//Set the value as a space
			tmp = *(c + ci);
			//Loop the rows
			for (i = 0; i < ROWSONCARD; i++) {
				//Loop the collumns
				for (j = 0; j < COLLUMNONCARD; j++) {
					if (j == 0) {
						//Write the first number into the file
						fprintf(ptr, "%d", tmp.rows[i][j]);
					}
					else {
						//Write the rest of the numbers into the file
						fprintf(ptr, ",%d", tmp.rows[i][j]);
					}
				}
				fprintf(ptr, "\n");
			}
			fprintf(ptr, "\n");
		}
		//Write a line brake into the file
		fprintf(ptr, "\n");
	}
}

//Function used to write the drawn numbers into the file
void writeDrawnNumbers(FILE* ptr,int* numbersDrawn) {
	//Iterator index
	int i;
	//If the file is open
	if (ptr != NULL) {
		//Write to the file
		for (i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) {
			//If it is the first line
			if (i == 0) {
				//Write the first number into the file
				fprintf(ptr, "%d", *(numbersDrawn+i));
			} else {
				//Write the rest of the numbers into the file
				fprintf(ptr, ",%d", *(numbersDrawn + i));
			}
		}
		//Write a line brake into the file
		fprintf(ptr, "\n");
	}
};

//Function used to save the game
struct file_list* saveGame(card* c, int sizeOfCards, int* numbersDrawn,char fileName[], struct file_list** loadedFile) {
	//Open the file
	FILE* ptr = openNewFileForWriting(fileName, loadedFile);
	//Write the drawn numbers
	writeDrawnNumbers(ptr,  numbersDrawn);
	//Write the crads
	writeCards(ptr, c, sizeOfCards);
	//Close the file
	fclose(ptr);
	return *loadedFile;
}

//Function used to scan the saved games dir
struct file_list* openSavedGamesDir() {
	//The first and curr file_list item
	struct file_list* head = NULL;
	struct file_list* curr = NULL;
	
	//Opened dir
	tinydir_dir dir;
	//Iterator index
	int i,n,a=0;
	char fileName[256];
	char fileExtension[4];
	//Open the dir
	if (tinydir_open_sorted(&dir, ".") != -1) {
		//Loop the dir
		for (i = 0; i < dir.n_files; i++) {
			//Create a new file
			tinydir_file file;
			//Get the file name
			tinydir_readfile_n(&dir, &file, i);

			//Get the file extension
			for (n = 0; n < 4; n++) fileExtension[n] = file.extension[n];

			//Check if it is a file and it has the bngextension
			if (!file.is_dir && strcmp(fileExtension,"bng")==0) {

				//Encountered a lovely little bug. 
				//file.name is a 256 long char but if i printf it with %s it will only return the first letter(same with srtcpy)
				//I have to manually copy the characters into a temporary holder.
				for (n = 0; n < 255; n++) {
					//Check if the string is terminated
					if (file.name[n] != '\n') {
						//Copy the character
						fileName[n] = file.name[n];
					}
					else {
						//Add terminator
						fileName[n] = '\n';
						//Exit the loop
						break;
					}
				}



				//Create a first item of the link
				struct file_list* ptr = (struct file_list*)malloc(sizeof(struct file_list));
				//Set the attributes of the item
				strcpy(ptr->name, fileName);
				ptr->index = ++a;
				ptr->next = NULL;

				//Add the name to the list
				if (head == NULL) {
					//Add the item to the first and the current
					head = curr = ptr;
				} else {
					//Link the item
					curr->next = ptr;
					//Replace the current item
					curr = ptr;
				}

			}
			
		}
		//Close the dir
		tinydir_close(&dir);
	}
	//Return the list
	return head;
}


//Function used to open a file for reading
FILE* openFileForReading(char file[]) {
	//Open a new file 
	return fopen(file, "r");
};
//Function used to get the drawn numbers
void readDrawnNumbers(FILE* ptr,  int* numbersDrawn) {
	//Iterator index
	int i;
	//If the file is open
	if (ptr != NULL) {
		//Loop the numbers
		for (i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) {
			fscanf(ptr, "%d,", (numbersDrawn+i));
		}
	}
}

//Function used to get the number of cards cards
int readCardNumber(FILE* ptr) {
	//Iterator index
	int cards=0;
	//If the file is open
	if (ptr != NULL) {
		fscanf(ptr, "%d", &cards);
	}
	return cards;
}
//Function used to get the number of cards cards
void readCards(FILE* ptr,card* cards,int numberOfCards) {
	//Iterator index
	int ci,i,j;
	//Card tmp
	card tmp;
	//If the file is open
	if (ptr != NULL) {
		//Loop Cards
		for (ci = 0; ci < numberOfCards; ci++) {
			//Create temporary card
			tmp=*(cards + ci);
			//Loop the rows
			for (i = 0; i < ROWSONCARD; i++) {
				//Loop the collumns
				for (j = 0; j < COLLUMNONCARD; j++) {
					if (j == 0) {
						//Write the first number into the file
						fscanf(ptr, "%d", &tmp.rows[i][j]);
						//printf("%d", tmp.rows[i][j]);
					}else {
						//Write the rest of the numbers into the file
						fscanf(ptr, ",%d", &tmp.rows[i][j]);
						//printf(",%d", tmp.rows[i][j]);
					}
				}
				//printf("\n");
			}
			//printf("\n");
			//Save the card
			cards[ci] = tmp;
		}
	}
	return cards;
}

//Function used to get the last of items in file_list
int gteLastIndexOfFileList(struct file_list* list) {
	//The index
	int i = 0;
	//If the list is empty then return -1
	if (list == NULL) return -1;
	//Loop the list
	while (list != NULL) {
		//Increase the counter
		i++;
		//Change to the next element
		list = list->next;
	}
	//Return the counter
	return i;
}

//Function used to get file_list item by index
struct file_list* getFileListItemByIndex(struct file_list* list, int index) {
	//If the list is empty then return empty
	if (list == NULL) return NULL;
	//Loop the list
	while (list != NULL) {
		//If the item is found
		if (list->index == index) return list;
		//Change to the next element
		list = list->next;
	}
	//Return the counter
	return NULL;
}
