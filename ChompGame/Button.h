/**#pragma once
#include "SFML/Graphics.hpp"
using namespace sf;
class Button
{
private:
	int posX = 0, posY = 0, width = 0, height;
	IntRect boundingBox;
	Sprite sprite;
public:
	Button(int x, int y, int w, int h, Sprite s)
	{
		posX = x;
		posY = y;
		width = w;
		height = h;
		sprite = sprite;
		boundingBox = IntRect(x, y, w, h);
		sprite.setPosition(x, y);
	}

	void drawButton(RenderWindow& app)
	{
		app.draw(sprite);
	}

	void setPosition(int x, int y)
	{
		posX = x;
		posY = y;
	}

	void setSize(int w, int h)
	{
		width = w;
		height = h;
	}
};
*/
