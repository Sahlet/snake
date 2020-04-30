#pragma once
#include <Windows.h>

namespace console_hacks
{
	// x is the column, y is the row. The origin (0,0) is top-left.
	void setCursorPosition(int x, int y);

	//crears screen hConsole
	void cls(HANDLE hConsole);

	//crears screen STD_OUTPUT_HANDLE
	void cls();
}