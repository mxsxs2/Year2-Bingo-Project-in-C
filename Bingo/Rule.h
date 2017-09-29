#ifndef RULE_H_
#define CRULE_H_

#include "Card.h"

//Function used to get the number of drawn numbers in a line
int checkNumberOfDrawnNumbersInLine(card, int, int*);
//Function used to get the remainig numbers to win one line
int checkLineRuleRemaining(card, int*);
//Function used to get the remaining numbers to win two line
int checkTwoLineRuleRemaining(card, int*);
//Function used to get the remainig numbers to win full house
int checkFullHouseRuleRemaining(card, int*);
//Function used to get the numbers needed to win the four corners rule
int checkCornersRuleRemainingNumbers(card, int*);
//Check four corner Rule
bool checkCornersRule(card, int*);
//Check line rule
bool checkLineRule(card, int, int*);
//Function used to check a card if there is a two line win on it
bool checkWin(card, int*, int);
//Modify card winning state
card modifyCardWinningState(card, int*);

#endif 
