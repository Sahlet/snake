// snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "StateProcessor.h"
#include "Menu.h"
#include "Game.h"

void run()
{
    auto terminator = std::make_shared<Terminator>();
    StateProcessor stateProcessor(std::static_pointer_cast<ITerminator>(terminator));

    int bestScore = 0, lastScore = 0;

    bool quit = false;
    while (!quit)
    {
        auto menuResult = menu(stateProcessor, bestScore, lastScore);
        switch (menuResult)
        {
        case MenuItem::Play:
            lastScore = play(stateProcessor);
            break;

        case MenuItem::Quit:
        default:
            quit = true;
            break;
        };

        bestScore = bestScore < lastScore ? lastScore : bestScore;
    }

    terminator->flag.store(true);

    stateProcessor.join();
}

int main()
{
    run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
