#pragma once
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

