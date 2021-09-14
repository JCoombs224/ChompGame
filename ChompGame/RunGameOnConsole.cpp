/********************************************
*	DEVELOPER:	Jamison Coombs
*	COLLABORATORS: Professor Walker
*	COURSE:	CS226 Discrete Structures
*	PROJECT:	<project #1>
*	LAST MODIFIED:	9/1/2021
********************************************/
/********************************************
*	Chomp.cpp
*********************************************
*	PROGRAM DESCRIPTION:
*	Chomp is a two-player strategy game played on a rectangular grid made up of smaller square cells, which
*	can be thought of as the blocks of a chocolate bar. The players take it in turns to choose one block
*	and "eat it" (remove from the board), together with those that are below it and to its right. The top
*	left block is "poisoned" and the player who eats this loses.
*********************************************
*	FUNCTIONS:
* + class Square
*   - playable:bool = true
*   + Square()
*   + chomp()
*   + isplayable():bool
* + class Board
*   - sizeX:unsigned int
*   - sizeY:unsinged int
*   - boardGrid:vector<Square>
*   + Board(unsigned int, unsigned int)
*   + doChomp(unsigned int, unsigned int):bool
*   + isGameWon():bool
*   + printBoard()
*********************************************/

/*********************************************
*	SETUP STATEMENTS and PURPOSE:
*********************************************/
#include <iostream> //to output to console
#include <string> // Allows string manipulation
#include <vector> // To create an expandable data structure of objects
#include "GameBoardLogic.h"
using namespace std;


/*********************************************
*	MAIN PROGRAM:
*********************************************/
int mainOld()
{

	// Instance variables
	bool continueProg = false;
	unsigned int columns, rows;
	string player1, player2, temp;

	// Introduction to the game
	cout << "Welcome to Chomp!\n";
	cout << "This game is played on a chocolate bar of dimension m x n, i.e. the bar is divided in n x m squares." << endl <<
		"At each turn the current player chooses a square and eats everything below and right of the chosen square." << endl <<
		"A player wins when the only square the next player can eat is the poisned square in the top-left." << "\n\n";

	// Get player names
	cout << "Enter Player 1's name: ";
	getline(cin, player1);

	cout << "Enter Player 2's name: ";
	getline(cin, player2);


	// Get the size of the game board (m x n)
	while (!continueProg)
	{
		cout << "Enter the number of columns (m) on the game board between 2-9: ";
		cin >> columns;
		if (columns > 1 && columns < 10)
			continueProg = true;
		else
			cerr << "Please give a valid input!\n\n";
	}
	continueProg = false;
	while (!continueProg)
	{
		cout << "Enter the number of rows (n) on the game board between 2-9: ";
		cin >> rows;
		if (rows > 1 && rows < 10)
			continueProg = true;
		else
			cerr << "Please give a valid input!\n\n";
	}
	continueProg = false;

	// Create a new game board
	GameBoardLogic gameBoard(columns, rows);

	// Set current player
	string currentPlayer = player1;
	string nextPlayer = player2;
	temp = player1;

	// Temp values for inputs
	int m, n;

	// Explain game to players
	cout << "\nHere is the game board." << endl <<
		"The top left coordinate is (1, 1) and the bottom right is (" << columns << ", " << rows << ").\n";
	cout << "The game is won when the only piece the next player can eat is the top left piece.\n\n";
	cout << "When entering a move type X followed by a space then Y.\n\n";
	cout << "O - Active and playable piece.\n. - Chomped and unplayable piece.\n\n";

	// Loops active game until a player has won
	while (!continueProg)
	{
		// Print the current game board
		gameBoard.printBoardToConsole();
		// Get current player's move
		while (!continueProg)
		{
			cout << currentPlayer << "'s move (m n): ";
			cin >> m >> n;

			// If valid move continue
			if (gameBoard.doChomp(m - 1, n - 1))
				continueProg = true;
		}
		continueProg = false;

		// Check for a winning move
		if (gameBoard.isGameWon())
		{
			break; // break loop if game is won
		}
		// Set new current player
		temp = currentPlayer;
		currentPlayer = nextPlayer;
		nextPlayer = temp;
	}

	// Print winning player
	gameBoard.printBoardToConsole();
	cout << currentPlayer << " wins!!\n\n";
	system("pause"); // wait for input to close program

	return 0;
}
