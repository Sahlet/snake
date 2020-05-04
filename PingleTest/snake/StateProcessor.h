#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include "Terminator.h"

//keyboard state
struct State
{
    bool empty = true;

    bool enterPressed = false;
    bool escPressed = false;

    //only one arrow can be pressed at time
    bool leftPressed = false;
    bool upPressed = false;
    bool rightPressed = false;
    bool downPressed = false;
};

//StateProcessor class provides last state
class StateProcessor
{
public:
    StateProcessor(std::shared_ptr<ITerminator> terminator);

    std::unique_ptr<State> getLastState();

    inline void join()
    {
        t.join();
    }

private:
    std::mutex m;
    std::thread t;
    State state;
};