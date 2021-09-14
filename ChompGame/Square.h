#pragma once

/**
* The square class represents a square on a game board is either playable or non-playable
* if it was chomped.
*/
class Square
{
		// Private class variables
	private:
		bool playable = true;
		// public class variables
	public:
		// Functions

		// Default constructor
		Square();

		// Chomp this square
		void chomp();

		// Check to see if this square is playable
		bool isPlayable();
};

