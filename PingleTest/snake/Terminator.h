#pragma once
#include <atomic>

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