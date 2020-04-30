#pragma once
#include "StateProcessor.h"

enum class MenuItem : int
{
	Play,
	Quit
};

//returns selected MenuItem
MenuItem menu(StateProcessor& stateProcessor, int bestScore, int lastScore);