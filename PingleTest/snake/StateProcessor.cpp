#include <Windows.h>
#include "StateProcessor.h"

void fillState(State& e)
{
    auto pressed = [](int vKey) -> bool
    {
        return GetAsyncKeyState(vKey);
    };

    bool wasPressed = false;
    wasPressed = e.enterPressed = pressed(VK_RETURN);
    if (wasPressed) { e.empty = false; return; }
    wasPressed = e.escPressed = pressed(VK_ESCAPE);
    if (wasPressed) { e.empty = false; return; }
    wasPressed = e.leftPressed = pressed(VK_LEFT);
    if (wasPressed) { e.empty = false; return; }
    wasPressed = e.upPressed = pressed(VK_UP);
    if (wasPressed) { e.empty = false; return; }
    wasPressed = e.rightPressed = pressed(VK_RIGHT);
    if (wasPressed) { e.empty = false; return; }
    wasPressed = e.downPressed = pressed(VK_DOWN);
    if (wasPressed) { e.empty = false; return; }
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

            if (!s.empty)
            {
                std::lock_guard<std::mutex> lock(m);
                this->state = s;
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
        state = State();
    }

    return std::move(res);
}