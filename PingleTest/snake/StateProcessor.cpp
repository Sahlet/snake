#include <Windows.h>
#include "StateProcessor.h"

void State::update(const State& s)
{
    *this = s;

    bool arrowPressed = false;

    arrowPressed |= leftPressed;
    if (arrowPressed)
    {
        upPressed = false;
    }

    arrowPressed |= upPressed;
    if (arrowPressed)
    {
        rightPressed = false;
    }

    arrowPressed |= rightPressed;
    if (arrowPressed)
    {
        downPressed = false;
    }
}

void fillState(State& e)
{
    auto pressed = [](int vKey) -> bool
    {
        return GetAsyncKeyState(vKey);
    };

    e.enterPressed = pressed(VK_RETURN);
    e.escPressed = pressed(VK_ESCAPE);
    e.leftPressed = pressed(VK_LEFT);
    e.upPressed = pressed(VK_UP);
    e.rightPressed = pressed(VK_RIGHT);
    e.downPressed = pressed(VK_DOWN);
}

StateProcessor::StateProcessor(std::shared_ptr<ITerminator> terminator)
{
    if (!terminator)
    {
        throw std::exception("terminator is null");
    }

    t = std::thread([this, terminator]() {
        while (true)
        {
            if (terminator->getTerminate())
            {
                return;
            }

            State s;
            fillState(s);

            {
                std::lock_guard<std::mutex> lock(m);
                this->state.update(s);
            }
        }
    });
}

std::unique_ptr<State> StateProcessor::getLastState()
{
    std::unique_ptr<State> res;
    {
        std::lock_guard<std::mutex> lock(m);
        res = std::make_unique<State>(state);
    }

    return std::move(res);
}