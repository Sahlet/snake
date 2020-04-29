#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include "Terminator.h"

struct State
{
    bool enterPressed = false;
    bool escPressed = false;
    bool leftPressed = false;
    bool upPressed = false;
    bool rightPressed = false;
    bool downPressed = false;

    void update(const State& s);
};

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