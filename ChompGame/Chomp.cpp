/********************************************
*	DEVELOPER:	Jamison Coombs
*	LAST MODIFIED:	9/14/2021
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
#include "SFML/Audio.hpp"
#include "GameBoardLogic.h"
using namespace std;
using namespace sf; // sfml namespace

void inputGameDimensions(unsigned int&, unsigned int&);
void animate(vector<vector<int>>&, int&, float, float&, int, int, int, int);


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

	srand((unsigned)time(NULL));

	// First get the game options from console (update later to also be GUI based)
	inputGameDimensions(boardW, boardH);

	// Create parameter for texture size with scaling
	int tW = TX_S * TEXTURE_SCALE_XY;

	// Create the game board logic
	GameBoardLogic GameBoard(boardW, boardH);

	// Create a render window
	RenderWindow window(VideoMode(880, 600), "Chomp", sf::Style::Titlebar | sf::Style::Close);
	Image icon;
	icon.loadFromFile("res/chompicon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// Get the size of the window
	Vector2u size = window.getSize();
	// Get the coordinates of the center of the board to center the view on the board
	float vCenterX = (boardW * tW) / 2;
	float vCenterY = (boardH * tW) / 2;
	View view(Vector2f(vCenterX, vCenterY), Vector2f(size.x, size.y));

	// Set the windows view
	window.setView(view);


	/************************
	* Window Objects
	************************/
	Texture t;
	t.loadFromFile(TEXTURE_NAME);
	Sprite s(t);
	// Scale sprite texture
	s.setScale(TEXTURE_SCALE_XY, TEXTURE_SCALE_XY);

	SoundBuffer buffer[2];
	if (!buffer[0].loadFromFile("res/sound/eat3.ogg"))
		return -1;
	if (!buffer[1].loadFromFile("res/sound/eat2.ogg"))
		return -1;

	Sound eatSnd;
	eatSnd.setBuffer(buffer[1]);

	// Create a 2d vector grid for the square sprites
	vector<vector<int>> sGrid;
	vector<vector<int>> grid;

	// Fill sprite grid with defualt square texture
	for (int i = 0; i < boardW; i++)
	{
		vector<int> tempV;

		for (int j = 0; j < boardH; j++)
		{
			tempV.push_back(1);
		}
		sGrid.push_back(tempV);
	}

	// Set first brick to poisned brick
	sGrid[0][0] = 0;

	// Animation variables - IM A BAD CODER
	bool doAnimate = false;
	int animationFrame = 10;
	float deltaTime = 0.00f, totalTime = 0.00f;
	Clock clock;
	int chompedX, chompedY;

	Font font;
	if (!font.loadFromFile(FONT_NAME))
	{
		cerr << "Unable to load font: " << FONT_NAME << endl;
		return -1;
	}

	// Text to represent current player
	Text playerTxt[2];
	playerTxt[0].setFont(font);
	playerTxt[1].setFont(font);
	playerTxt[0].setString("Player 1's\n   Turn");
	playerTxt[1].setString("Player 2's\n   Turn");
	playerTxt[0].setCharacterSize(28);
	playerTxt[1].setCharacterSize(28);
	playerTxt[0].setFillColor(Color::Red);
	playerTxt[1].setFillColor(Color::Green);
	playerTxt[0].setOutlineColor(Color::Black);
	playerTxt[1].setOutlineColor(Color::Black);
	playerTxt[0].setOutlineThickness(3);
	playerTxt[1].setOutlineThickness(3);
	playerTxt[0].setStyle(Text::Bold);
	playerTxt[1].setStyle(Text::Bold);
	playerTxt[0].setPosition(-140, size.y / 200);
	playerTxt[1].setPosition((vCenterX * 2) + 4, 0);

	// 

	window.setVerticalSyncEnabled(true);

	bool player1Turn = true;

	// Run window
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		// Get mouse position
		Vector2i mPosWindow = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(mPosWindow);
		int x = pos.x / tW;
		int y = pos.y / tW;
		
		// Event handler
		Event e;

		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();

			// On left click check what square is clicked on and execute
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				{
					if (GameBoard.doChomp(x, y))
					{
						chompedX = x;
						chompedY = y;
						animationFrame = 1;

						eatSnd.setBuffer(buffer[rand() % 2]);
						eatSnd.setPitch((float)(rand() % 100 + 75) / 100);
						eatSnd.play();

						if (GameBoard.isGameWon())
						{
							window.close();
						}
						else
						{
							player1Turn ? player1Turn = false : player1Turn = true;
						}
					}
				}

		}

		window.clear(sf::Color::Color(18, 77, 122));

		if(animationFrame < 5)
			animate(sGrid, animationFrame, deltaTime, totalTime, chompedX, chompedY, boardW, boardH);

		// Draw game board
		for (int i = 0; i < boardW; i++)
			for (int j = 0; j < boardH; j++)
			{
				s.setTextureRect(IntRect(sGrid[i][j] * TX_S, 0, TX_S, TX_S));
				s.setPosition(i * tW, j * tW);
				window.draw(s);
			}

		// Draw the player whos turn it is
		if (player1Turn)
			window.draw(playerTxt[0]);
		else
			window.draw(playerTxt[1]);

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

void animate(vector<vector<int>>& grid, int& frame, float deltaTime, float& totalTime, int x, int y, int w, int h)
{
	float switchTime = 0.03;
	totalTime += deltaTime;

	if (totalTime >= switchTime)
	{
		for (int i = x; i < w; i++)
		{
			for (int j = y; j < h; j++)
				if(grid[i][j] < 4) 
					grid[i][j] = frame;
		}
		frame++;
		totalTime -= switchTime;
	}

}
