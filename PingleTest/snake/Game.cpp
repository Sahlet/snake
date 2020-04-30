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

void print(int score, std::string prevBuffer[MAX_X][MAX_Y], std::string buffer[MAX_X][MAX_Y])
{
	std::stringstream s;
	s << "Score: " << score;
	std::string scoreSting = s.str();
	console_hacks::setCursorPosition(0, 0);
	std::cout << scoreSting;

	console_hacks::setCursorPosition(scoreSting.size(), 0);

	std::cout << std::string(MAX_X - scoreSting.size(), ' ');

	//console_hacks::setCursorPosition(bufferOriginX, bufferOriginY);

	for (int i = 0; i < MAX_X; i++)
	{
		for (int j = 0; j < MAX_Y; j++)
		{
			if (prevBuffer[i][j] != buffer[i][j])
			{
				console_hacks::setCursorPosition(bufferOriginX + i, bufferOriginY + j);
				std::cout << buffer[i][j];
			}
		}
	}
}

int play(StateProcessor& stateProcessor)
{
	printingFrame();

	std::string prevBuffer[MAX_X][MAX_Y];
	std::string buffer[MAX_X][MAX_Y];

	return 0;
}