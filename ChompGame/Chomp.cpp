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
using namespace std;
using namespace sf; // sfml namespace


/*********************************************
*	MAIN PROGRAM:
*********************************************/
int main()
{
	RenderWindow app(VideoMode(800, 800), "test");
	sf::Event e;
	Font font;
	font.loadFromFile("res/font/arial.ttf");
	Text text;
	text.setFont(font);
	text.setString("Welcome to Chomp!");

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top);
	text.setPosition(sf::Vector2f(800 / 2.0f, 10));

	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);

	app.setFramerateLimit(1);

	while (app.isOpen())
	{
		while (app.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				app.close();

			app.clear(sf::Color::White);
			app.draw(text);
			app.display();
		}

		//app.clear(Color::White);

	}


	return 0;
}
