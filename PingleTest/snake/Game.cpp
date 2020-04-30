#include <sstream>
#include <iostream>
#include "Game.h"
#include "ConsoleHacks.h"

const int MAX_X = 50, MAX_Y = 20;
const int frameOriginX = 4, frameOriginY = 2;
const int bufferOriginX = frameOriginX + 1, bufferOriginY = frameOriginY + 1;

void printingFrame()
{
	console_hacks::setCursorPosition(frameOriginX, frameOriginY);
	std::cout << "\xc9";
	for (size_t i = 0; i < MAX_X; i++)
	{
		std::cout << "\xcd";
	}
	std::cout << "\xbb";
	
	for (size_t i = 0; i < MAX_Y; i++)
	{
		console_hacks::setCursorPosition(frameOriginX, frameOriginY + i + 1);
		std::cout << "\xba" << std::string(MAX_X, ' ') << "\xba";
	}
	
	console_hacks::setCursorPosition(frameOriginX, frameOriginY + MAX_Y + 1);
	std::cout << "\xc8";
	for (size_t i = 0; i < MAX_X; i++)
	{
		std::cout << "\xcd";
	}
	std::cout << "\xbc";
}

void print(int score, char prevBuffer[MAX_X][MAX_Y], char buffer[MAX_X][MAX_Y])
{
	std::stringstream s;
	s << "Score: " << score;
	std::string scoreSting = s.str();
	console_hacks::setCursorPosition(0, 0);
	std::cout << scoreSting;

	console_hacks::setCursorPosition(scoreSting.size(), 0);

	std::cout << std::string(MAX_X - scoreSting.size(), ' ');

	for (int i = scoreSting.size(); i < MAX_X; i++)
	{
		console_hacks::setCursorPosition(i, 0);
	}

	//console_hacks::setCursorPosition();
}

int play(StateProcessor& stateProcessor)
{
	return 0;
}