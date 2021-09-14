/********************************************
*	DEVELOPER:	Jamison Coombs
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
/*********************************************
*	SETUP STATEMENTS and PURPOSE:
*********************************************/
#include <iostream> //to output to console
#include <string> // Allows string manipulation
#include <vector> // To create an expandable data structure of objects
#include "SFML/Graphics.hpp"
#include "GameBoardLogic.h"
using namespace std;
using namespace sf; // sfml namespace

void inputGameDimensions(unsigned int&, unsigned int&);

/*********************************************
*	MAIN PROGRAM:
*********************************************/
int main()
{
	// CONSTANTS
	const string FONT_NAME = "res/font/arial.ttf";
	const string TEXTURE_NAME = "res/texture/bricks.png";
	const int TX_S = 16;
	const int TEXTURE_SCALE_XY = 3.f;

	// Instance Variables
	unsigned int boardW = 8, boardH = 8; // Game board is 8x8 by defualt

	// First get the game options from console (update later to also be GUI based)
	inputGameDimensions(boardW, boardH);

	// Create parameter for texture size with scaling
	int tW = TX_S * TEXTURE_SCALE_XY;

	// Create the game board logic
	GameBoardLogic GameBoard(boardW, boardH);


	srand(time(0));

	// Create a render window
	RenderWindow window(VideoMode(600, 600), "Chomp");

	// Get the size of the window
	Vector2u size = window.getSize();
	// Get the coordinates of the center of the board to center the view on the board
	float vCenterX = (boardW * tW) / 2;
	float vCenterY = (boardH * tW) / 2;
	View view(Vector2f(vCenterX, vCenterY), Vector2f(size.x, size.y));

	//view.move();
	//view.zoom(-3.f);
	window.setView(view);

	Texture t;
	t.loadFromFile(TEXTURE_NAME);
	Sprite s(t);

	
	// Scale sprite texture
	s.setScale(TEXTURE_SCALE_XY, TEXTURE_SCALE_XY);

	// Create a 2d vector grid for the square sprites
	vector<vector<int>> sGrid;
	vector<vector<int>> grid;

	// Fill sprite grid with defualt square texture
	for (int i = 0; i < boardW; i++)
	{
		vector<int> tempV;

		for (int j = 0; j < boardH; j++)
		{
			tempV.push_back(0);
		}
		sGrid.push_back(tempV);
	}

	// Set first brick to poisned brick
	sGrid[0][0] = 1;

	// Run window
	while (window.isOpen())
	{
		// Get mouse position
		Vector2i mPosWindow = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(mPosWindow);
		int x = pos.x / tW;
		int y = pos.y / tW;
		
		// Event handler
		Event e;

		window.setVerticalSyncEnabled(true);

		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();

			// On left click check what square is clicked on and execute
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				{
					if (GameBoard.getSquareAt(x, y).isPlayable())
					{
						GameBoard.doChomp(x, y);
						for (int i = x; i < boardW; i++)
						{
							for (int j = y; j < boardH; j++)
								sGrid[i][j] = 5;
						}
						if (GameBoard.isGameWon())
						{
							window.close();
						}
					}
				}
		}

		window.clear(sf::Color::White);

		// Draw game board
		for (int i = 0; i < boardW; i++)
			for (int j = 0; j < boardH; j++)
			{
				s.setTextureRect(IntRect(sGrid[i][j] * TX_S, 0, TX_S, TX_S));
				s.setPosition(i * tW, j * tW);
				window.draw(s);
			}

		window.display();

	}

	cout << "GAME OVER!\n\n";

	return 0;
}

void inputGameDimensions(unsigned int& x, unsigned int& y)
{
	// bool for loop
	bool continueProg = false;

	// Display welcome message
	cout << "Welcome to Chomp!\n";
	cout << "This game is played on a chocolate bar of dimension m x n, i.e. the bar is divided in n x m squares." << endl <<
		"At each turn the current player chooses a square and eats everything below and right of the chosen square." << endl <<
		"A player wins when the only square the next player can eat is the poisned square in the top-left." << "\n\n";

	// Get the size of the game board (m x n)
	while (!continueProg)
	{
		cout << "Enter the number of columns (m) on the game board between 2-12: ";
		cin >> x;
		if (x > 1 && x < 13)
			continueProg = true;
		else
			cerr << "Please give a valid input!\n\n";
	}
	continueProg = false;
	while (!continueProg)
	{
		cout << "Enter the number of rows (n) on the game board between 2-12: ";
		cin >> y;
		if (y > 1 && y < 13)
			continueProg = true;
		else
			cerr << "Please give a valid input!\n\n";
	}
}
