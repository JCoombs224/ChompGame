#include "Square.h"

// Constructor
Square::Square() {}

// Getter
bool Square::isPlayable()
{
	return playable;
}

// Chomp piece (set playable = false)
void Square::chomp()
{
	playable = false;
}