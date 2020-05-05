#include <sstream>
#include <iostream>
#include <vector>
#include "Game.h"
#include "ConsoleHacks.h"
#include "Snake.h"

const int MAX_X = 50, MAX_Y = 20;
const int frameOriginX = 4, frameOriginY = 2;
const int bufferOriginX = frameOriginX + 1, bufferOriginY = frameOriginY + 1;

void printingFrame()
{
	console_hacks::setCursorPosition(frameOriginX, frameOriginY);
	std::cout << "\xc9";
	for (int i = 0; i < MAX_X; i++)
	{
		std::cout << "\xcd";
	}
	std::cout << "\xbb";
	
	for (int i = 0; i < MAX_Y; i++)
	{
		console_hacks::setCursorPosition(frameOriginX, frameOriginY + i + 1);
		std::cout << "\xba" << std::string(MAX_X, ' ') << "\xba";
	}
	
	console_hacks::setCursorPosition(frameOriginX, frameOriginY + MAX_Y + 1);
	std::cout << "\xc8";
	for (int i = 0; i < MAX_X; i++)
	{
		std::cout << "\xcd";
	}
	std::cout << "\xbc";
}

void print(int score, std::string* prevBuffer, std::string* buffer, unsigned short bufWidth, unsigned short bufHeight)
{
	std::stringstream s;
	s << "Score: " << score;
	std::string scoreSting = s.str();
	console_hacks::setCursorPosition(0, 0);
	std::cout << scoreSting;

	console_hacks::setCursorPosition((int)scoreSting.size(), 0);

	std::cout << std::string(MAX_X - scoreSting.size(), ' ');

	//console_hacks::setCursorPosition(bufferOriginX, bufferOriginY);

	for (int i = 0; i < bufWidth; i++)
	{
		for (int j = 0; j < bufHeight; j++)
		{
			if (prevBuffer[j * bufWidth + i] != buffer[j * bufWidth + i])
			{
				console_hacks::setCursorPosition(bufferOriginX + i, bufferOriginY + j);
				std::cout << buffer[j * bufWidth + i];
			}
		}
	}
}

bool willGrowByNextMove(const snake::Snake& snake, const snake::Food& food)
{
	auto nextHeadCoordinates = snake.getNextHeadCoordinates();
	auto coordinates = food.getCoordinates();

	return nextHeadCoordinates == coordinates;
}

int play(StateProcessor& stateProcessor)
{
	printingFrame();

	std::vector<std::string> prevBuffer(MAX_X * MAX_Y);
	std::vector<std::string> buffer(MAX_X * MAX_Y);

	int score = 0;
	snake::Snake snake;
	snake::Food food;
	food.jump(snake, MAX_X, MAX_Y);

	bool runGame = true;
	while (runGame)
	{
		auto state = stateProcessor.getLastState();

		if (state->escPressed)
		{
			break;
		}

		bool arrowPressed = state->leftPressed || state->upPressed || state->rightPressed || state->downPressed;
		if (arrowPressed)
		{
			//only one arrow can be pressed at time
			snake::Direction dir = snake::Direction::Left;
			if (state->leftPressed)
			{
				dir = snake::Direction::Left;
			}
			else if (state->upPressed)
			{
				dir = snake::Direction::Up;
			}
			else if (state->rightPressed)
			{
				dir = snake::Direction::Right;
			}
			else if (state->downPressed)
			{
				dir = snake::Direction::Down;
			}

			snake.turn(dir);
		}

		if (willGrowByNextMove(snake, food))
		{
			snake.grow();
			score++;
			if (!food.jump(snake, MAX_X, MAX_Y))
			{
				runGame = false;
			}
		}
		else
		{
			snake.move();
		}

		food.printToBuffer(buffer.data(), MAX_X, MAX_Y);
		snake.printToBuffer(buffer.data(), MAX_X, MAX_Y);

		print(score, prevBuffer.data(), buffer.data(), MAX_X, MAX_Y);

		for (int i = 0; i < MAX_X; i++)
		{
			for (int j = 0; j < MAX_Y; j++)
			{
				prevBuffer[j * MAX_X + i] = std::move(buffer[j * MAX_X + i]);
				buffer[j * MAX_X + i] = " ";
			}
		}

		console_hacks::setCursorPosition(bufferOriginX + MAX_X + 2, bufferOriginY + MAX_Y + 2);
		std::this_thread::sleep_for(std::chrono::seconds(1));

		auto headCoordinates = snake.getHeadCoordinates();
		
		if (headCoordinates.first >= MAX_X || headCoordinates.second >= MAX_Y)
		{
			runGame = false;
		}

		if (snake.isHeadOnTail())
		{
			runGame = false;
		}
	}

	return score;
}