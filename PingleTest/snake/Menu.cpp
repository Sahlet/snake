#include <iostream>
#include <sstream>
#include <cstdio>
#include "Menu.h"
#include "ConsoleHacks.h"

MenuItem menu(StateProcessor& stateProcessor, int bestScore, int lastScore)
{
	struct ScreenGuard
	{
		~ScreenGuard()
		{
			console_hacks::cls();
		}
	} screenGuard;

	struct SyncWithStdioGuard
	{
		SyncWithStdioGuard()
		{
			prevValue = std::ios::sync_with_stdio(false);
		}

		~SyncWithStdioGuard()
		{
			std::ios::sync_with_stdio(prevValue);
		}

	private:
		bool prevValue;
	} syncWithStdioGuard;

	const char* menuElemnts[] =
	{
		"Play", "Quit"
	};
	auto space = "   ";
	auto leftSideArrow = "-> ";
	auto rightSideArrow = " <-";

	int curMenuElem = 0;
	bool update = true;
	
	while (true)
	{
		if (update)
		{
			std::stringstream ostream;

			ostream << "Best score: " << bestScore << std::endl
				<< "Last score: " << lastScore << std::endl;
			for (size_t i = 0; i < 5; i++)
			{
				ostream << std::endl;
			}

			for (size_t i = 0; i < sizeof(menuElemnts) / sizeof(menuElemnts[0]); i++)
			{
				ostream << std::string(10, ' ')
					<< (i == curMenuElem ? leftSideArrow : space)
					<< menuElemnts[i]
					<< (i == curMenuElem ? rightSideArrow : space)
					<< std::endl;
			}

			//console_hacks::cls();
			std::system("CLS");
			std::cout << ostream.str();
			std::cout.flush();
		}

		update = false;
		auto state = stateProcessor.getLastState();
		if (state->enterPressed)
		{
			switch (curMenuElem)
			{
			case 0:
				return MenuItem::Play;
			case 1:
			default:
				return MenuItem::Quit;
			}
		}
		else if (state->upPressed)
		{
			if (curMenuElem > 0)
			{
				update = true;
				curMenuElem--;
			}
		}
		else if (state->downPressed)
		{
			if (curMenuElem < ((sizeof(menuElemnts) / sizeof(menuElemnts[0])) - 1))
			{
				update = true;
				curMenuElem++;
			}
		}
	}

	return MenuItem::Quit;
}