#pragma once
#include <atomic>

//indicates whether the thread should stop running
class ITerminator
{
public:
    virtual bool getTerminate() = 0;
    virtual ~ITerminator() = default;
};

class Terminator : public ITerminator
{
public:
    virtual bool getTerminate();

public:
    std::atomic<bool> flag;
};