#pragma once
#pragma once
#include <vector>
#include "square.h"
using namespace std;

/**
* The board class contains all the info needed to play a game of chomp.
* It takes an m and n value and creates a board of squares of (m x n) size.
*/
class GameBoardLogic
{
	// Private instance variables
private:
	unsigned int sizeX, sizeY;
	// Initialize vector for the game board
	vector<vector<Square>> boardGrid;
public:
	// Constructor (takes the amount of columns and rows and creates a new chomp board)
	GameBoardLogic(unsigned int m, unsigned int n);

	// Execute chomp move on the board
	bool doChomp(unsigned int x, unsigned int y);

	// Check to see if the game has been won
	bool isGameWon();

	// Print the game board with playable squares as 'O' and nonplayable as 'X'
	void printBoardToConsole();
	};


