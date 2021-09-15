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

void inputGameDimensions(unsigned int&, unsigned int&, bool);
void animateChomp(vector<vector<int>>&, int&, float, float&, int, int, int, int, bool&);
void animateReset(vector<vector<int>>&, int&, float, float&, int, int, int, int, bool&, bool&);


/*********************************************
*	MAIN PROGRAM:
*********************************************/
int main()
{
	// CONSTANTS
	const string FONT_NAME = "res/font/arial.ttf";
	const string ICON_IMAGE_NAME = "res/chompicon.png";
	const string SQUARE_TEXTURE_NAME = "res/texture/bricks.png";
	const string BUTTON_TEXTURE_NAME = "res/texture/buttons.png";
	const int TX_S = 16; 
	const int TEXTURE_SCALE_XY = 3;

	// Instance Variables
	unsigned int boardW = 8, boardH = 8; // Game board is 8x8 by defualt
	bool restartGame = false;
	srand((unsigned)time(NULL));

	do
	{
		bool resetBoard = false;

		// First get the game options from console (update later to also be GUI based)
		inputGameDimensions(boardW, boardH, restartGame);
		restartGame = false;

		// Create parameter for texture size with scaling
		int tW = TX_S * TEXTURE_SCALE_XY;

		// Create a render window
		RenderWindow window(VideoMode(880, 600), "Chomp", sf::Style::Titlebar | sf::Style::Close);
		Image icon;
		icon.loadFromFile(ICON_IMAGE_NAME);
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// Get the size of the window
		Vector2u size = window.getSize();
		// Get the coordinates of the center of the board to center the view on the board
		float vCenterX = (boardW * tW) / 2;
		float vCenterY = (boardH * tW) / 2;

		// Create view for game board
		View gameView(Vector2f(vCenterX, vCenterY), Vector2f(size.x, size.y));

		// Set the windows view
		window.setView(gameView);


		/************************
		* Window Objects
		************************/

		// ************** Sprite Objects ****************

		Texture tSqr, tBtn;

		// Load in square sprite
		tSqr.loadFromFile(SQUARE_TEXTURE_NAME);
		Sprite sSqr(tSqr);
		// Scale sprite texture
		sSqr.setScale(TEXTURE_SCALE_XY, TEXTURE_SCALE_XY);

		// Load in close button sprite
		tBtn.loadFromFile(BUTTON_TEXTURE_NAME);
		Sprite sCloseButton(tBtn);
		// Scale sprite texture
		sCloseButton.setTextureRect(IntRect(0, 0, TX_S, TX_S));
		sCloseButton.setPosition(50, window.getSize().y - 100);
		sCloseButton.setScale(4, 4);

		// Load in restart button sprite
		Sprite sResetButton(tBtn);
		sResetButton.setScale(4, 4);
		sResetButton.setTextureRect(IntRect(2 * TX_S, 0, TX_S, TX_S));
		sResetButton.setPosition(window.getSize().x - 114, window.getSize().y - 100);


		// ************** Sound objects ******************

		SoundBuffer eatBuffer[2];
		if (!eatBuffer[0].loadFromFile("res/sound/eat3.ogg"))
			return -1;
		if (!eatBuffer[1].loadFromFile("res/sound/eat2.ogg"))
			return -1;
		Sound eatSnd;
		eatSnd.setBuffer(eatBuffer[1]);

		SoundBuffer winBuffer[2];
		if (!winBuffer[0].loadFromFile("res/sound/orb.ogg"))
			return -1;
		if (!winBuffer[1].loadFromFile("res/sound/explode1.ogg"))
			return -1;
		Sound winSnd;
		winSnd.setBuffer(winBuffer[0]);

		// ************** Text objects ******************

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


		// ************** Variables ******************
		bool doAnimate = false;
		int animationFrame = 1;
		float deltaTime = 0.00f, totalTime = 0.00f;
		Clock clock;
		int chompedX, chompedY;

		
		// ************** Fill Grid ******************

		// Create the game board logic
		GameBoardLogic GameBoard(boardW, boardH);
		
		// Create a 2d vector grid for the square sprites
		vector<vector<int>> sGrid;

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

		// Enable VSync to monitor refresh rate
		window.setVerticalSyncEnabled(true);

		// Set first player as player 1
		bool player1Turn = true;

		// ************** Run ChompGame Window **************
		while (window.isOpen())
		{
			// Get time between frames
			deltaTime = clock.restart().asSeconds();

			window.setView(gameView);

			// Get mouse position
			Vector2i mPosWindow = Mouse::getPosition(window);
			Vector2f pos = window.mapPixelToCoords(mPosWindow);
			int x = pos.x / tW;
			int y = pos.y / tW;

			// Event handler
			Event e;

			// Check for event
			while (window.pollEvent(e))
			{
				// On window close button pressed
				if (e.type == sf::Event::Closed)
					window.close();

				// On left click check what square is clicked on and execute
				if (e.type == Event::MouseButtonPressed)
					if (e.key.code == Mouse::Left)
					{
						// Check if close button was pressed
						if (sCloseButton.getGlobalBounds().contains(mPosWindow.x, mPosWindow.y))
						{
							restartGame = true;
							window.close();
						}
						// Check if reset button was pressed
						else if (sResetButton.getGlobalBounds().contains(mPosWindow.x, mPosWindow.y))
						{
							resetBoard = true;
							GameBoard.resetBoard(); // resets game board
							playerTxt[0].setString("Player 1's\n   Turn");
							playerTxt[1].setString("Player 2's\n   Turn");
							winSnd.setBuffer(winBuffer[0]);
							winSnd.play();
							doAnimate = true;
						}
						// Try to execute chomp move at mouse click location
						else if (GameBoard.doChomp(x, y))
						{
							chompedX = x;
							chompedY = y;
							doAnimate = true;

							// Play random chomp sound from the buffer
							eatSnd.setBuffer(eatBuffer[rand() % 2]);
							eatSnd.setPitch((float)(rand() % 100 + 75) / 100);
							eatSnd.play();

							// Check if the game has been won after chomp
							if (GameBoard.isGameWon())
							{
								// Determine which player won and show text according
								player1Turn ? playerTxt[0].setString("Player 1\n  Wins") :
									playerTxt[1].setString("Player 2\n  Wins");

								winSnd.play(); // Play win sound
							}
							// If poisned piece is eaten at the end of the game
							// NOTE: doChomp wont let poisned piece be eaten unless the game has been won
							if (x == 0 && y == 0)
							{
								winSnd.setBuffer(winBuffer[1]);
								winSnd.play();
							}
							player1Turn ? player1Turn = false : player1Turn = true;
						}
					}
			}

			// ************** Frame Drawing ***************

			// Clear window and begin drawing frame
			window.clear(sf::Color::Color(18, 77, 122));

			// Check to see if chomp animation needs to run
			if (doAnimate)
			{
				if(!resetBoard)
					animateChomp(sGrid, animationFrame, deltaTime, totalTime, chompedX, chompedY, boardW, boardH, doAnimate);
				else
					animateReset(sGrid, animationFrame, deltaTime, totalTime, chompedX, chompedY, boardW, boardH, doAnimate, resetBoard);
			}
			

			// Draw game board
			for (int i = 0; i < boardW; i++)
				for (int j = 0; j < boardH; j++)
				{
					sSqr.setTextureRect(IntRect(sGrid[i][j] * TX_S, 0, TX_S, TX_S));
					sSqr.setPosition(i * tW, j * tW);
					window.draw(sSqr);
				}

			// Draw the player whos turn it is
			if (player1Turn)
				window.draw(playerTxt[0]);
			else
				window.draw(playerTxt[1]);

			// Set the view to defualt to draw interface
			window.setView(window.getDefaultView());
			// Draw buttons
			window.draw(sCloseButton);
			window.draw(sResetButton);


			window.display();

		}

		cout << "GAME OVER!\n\n";
	}
	while (restartGame);


	return 0;
}

void inputGameDimensions(unsigned int& x, unsigned int& y, bool wasReset)
{
	// bool for loop
	bool continueProg = false;

	if (!wasReset)
	{
		// Display welcome message
		cout << "Welcome to Chomp!\n";
		cout << "This game is played on a chocolate bar of dimension m x n, i.e. the bar is divided in n x m squares." << endl <<
			"At each turn the current player chooses a square and eats everything below and right of the chosen square." << endl <<
			"A player wins when the only square the next player can eat is the poisned square in the top-left." << "\n\n";
	}

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

void animateChomp(vector<vector<int>>& grid, int& frame, float deltaTime, float& totalTime, int x, int y, int w, int h, bool& animate)
{
	float switchTime = 0.03;
	totalTime += deltaTime;

	if (frame > 4) // check if animation is done
	{
		animate = false;
		frame = 1;
		return; // break from function
	}
	if (totalTime >= switchTime)
	{
		for (int i = x; i < w; i++)
		{
			for (int j = y; j < h; j++)
				if (grid[i][j] < 4)
						grid[i][j] = frame;
		}
		frame++;
		totalTime -= switchTime;
	}
}

void animateReset(vector<vector<int>>& grid, int& frame, float deltaTime, float& totalTime, int x, int y, int w, int h, bool& animate, bool& reset)
{
	float switchTime = 0.03;
	totalTime += deltaTime;
	
	if (frame > 4) // check if animation is done
	{
		grid[0][0] = 0; // Reset poison brick sprite
		animate = false; // Disable animation
		reset = false; // Set reset to false since board is done resetting
		frame = 1; // Reset animation frame to 1
		return; // break from function
	}
	if (totalTime >= switchTime)
	{
		for (int i = 0; i < w; i++) // Animate each grid piece
		{
			for (int j = 0; j < h; j++)
				if (grid[i][j] > 1)
					grid[i][j] = 4 - frame;
		}
		frame++;
		totalTime -= switchTime;
	}
}