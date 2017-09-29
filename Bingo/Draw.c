#include "Config.h"
#include "Card.h"

//Function is used to draw a number
bool drawNumber(int* numbersDrawn) {
	//Iterator index
	int i;
	//Last empty index
	int lastEmptyIndex = -1;
	//Loop the numbers to find the last index space
	for (i = 0; i < MAXIMUMDRAWABLENUMBERS; i++) {
		//Check if the index is empty
		if (*(numbersDrawn + i) == 0) {
			//Marke the empty index
			lastEmptyIndex = i;
			//Exit the loop
			break;
		}
	}

	//Check if there is any space in the array
	if (lastEmptyIndex != -1) {
		//Create a new random number within the range and save it to the empty index
		numbersDrawn[lastEmptyIndex] = randomNumber(numbersDrawn, MAXIMUMDRAWABLENUMBERS, MAXIMUMDRAWABLENUMBERS, 1);
		//Return true on draw
		return true;
	}
	//Return false if cannot draw more;
	return false;
}

