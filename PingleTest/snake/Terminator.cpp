#include "Terminator.h"

bool Terminator::getTerminate()
{
    return flag.load();
}