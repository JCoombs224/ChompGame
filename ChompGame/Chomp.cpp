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
#include <vector> // To create an expandable data structure of objects
using namespace std; //access namespace


/*********************************************
*	CLASSES/FUNCTIONS:
*********************************************/
/**DESCRIPTION: <function description> */
//function definition

/**
* The square class represents a square on a game board is either playable or non-playable
* if it was chomped,
*/
class Square
{
	// Private class variables
private:
	bool playable = true;
	// public class variables
public:
	// Default constructor
	Square() {}

	// Chomp this square
	void chomp()
	{
		playable = false;
	}

	// Check to see if this square is playable
	bool isPlayable()
	{
		return playable;
	}
};

/**
* The board class contains all the info needed to play a game of chomp.
* It takes an m and n value and creates a board of squares of (m x n) size.
*/
class Board
{
	// Private instance variables
private:
	unsigned int sizeX, sizeY;
	// Initialize vector for the game board
	vector<vector<Square>> boardGrid;
public:
	// Constructor (takes the amount of columns and rows and creates a new chomp board)
	Board(unsigned int m, unsigned int n)
	{
		// Set the private size variables to the info given
		sizeY = n;
		sizeX = m;

		// Create a 2d vector of game squares to create a grid
		for (int i = 0; i < sizeY; i++)
		{
			// Vector to store column game squares
			vector<Square> v1;

			// Fill columns of new game squares
			for (int j = 0; j < sizeX; j++)
			{
				// Add square to column vector
				v1.push_back(Square());
			}
			// Add column to current row
			boardGrid.push_back(v1);
		}


	}

	// Execute chomp move on the board
	bool doChomp(unsigned int x, unsigned int y)
	{
		// First check if move is valid, if not return false
		if (x < 0 || y < 0 || (x == 0 && y == 0) || x >= sizeX || y >= sizeY || (!boardGrid[y][x].isPlayable()))
		{
			cerr << "That move is not playable, try again...\n";
			return false;
		}

		// Execute move and return true
		for (int i = y; i < sizeY; i++)
		{
			for (int j = x; j < sizeX; j++)
			{
				boardGrid[i][j].chomp();
			}
		}
		return true;
	}

	// Check to see if the game has been won
	bool isGameWon()
	{
		if ((!boardGrid[0][1].isPlayable()) && (!boardGrid[1][0].isPlayable()))
			return true;
		else
			return false;
	}

	// Print the game board with playable squares as 'O' and nonplayable as 'X'
	void printBoard()
	{
		cout << "   ";
		for (int i = 0; i < sizeX; i++)
		{
			cout << i+1 << ' ';
		}
		cout << "(m)" << endl;

		for (int i = 0; i < sizeY; i++)
		{
			cout << ' ' << i + 1 << ' ';
			for (int j = 0; j < sizeX; j++)
			{
				if (boardGrid[i][j].isPlayable())
					cout << "O ";
				else
					cout << ". ";
			}
			cout << endl;
		}
		cout << "(n)\n\n";
	}
};


/*********************************************
*	MAIN PROGRAM:
*********************************************/
int main()
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
	cin >> player1;

	cout << "Enter Player 2's name: ";
	cin >> player2;


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
	Board gameBoard(columns, rows);

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
		gameBoard.printBoard();
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
	gameBoard.printBoard();
	cout << currentPlayer << " wins!!\n\n";
	system("pause"); // wait for input to close program

	return 0;
}
