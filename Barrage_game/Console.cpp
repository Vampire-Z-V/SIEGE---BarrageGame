#include "Console.h"
#include <iostream>
using namespace std;


Console::Console()
{
	consolebuf.clear();
	fontinfoex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontinfoex);
	
}

HANDLE Console::getNowConsoleBuf()
{
	if (!consolebuf.empty())
		return consolebuf[consolebuf.size() - 1];
	else
		exit(EXIT_FAILURE);
}

void Console::createNewConsoleBuf(COORD cdFontSize, SMALL_RECT srWindowRect)
{
	fontinfoex.dwFontSize.X = cdFontSize.X;
	fontinfoex.dwFontSize.Y = cdFontSize.Y;

	COORD coordScreen = { srWindowRect.Right + 1,srWindowRect.Bottom + 1 };
	DWORD dwNumberOfCharsWritten;

	consolebuf.push_back(CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 
												   0, 
												   NULL, 
												   CONSOLE_TEXTMODE_BUFFER, 
												   NULL));

	SetConsoleScreenBufferSize(consolebuf[consolebuf.size()-1], coordScreen);
	SetConsoleWindowInfo(consolebuf[consolebuf.size() - 1], TRUE, &srWindowRect);
	SetCurrentConsoleFontEx(consolebuf[consolebuf.size() - 1], false, &fontinfoex);
	FillConsoleOutputAttribute(consolebuf[consolebuf.size() - 1], FOREGROUND_GREEN | FOREGROUND_INTENSITY, coordScreen.X*coordScreen.Y, { 0,0 }, &dwNumberOfCharsWritten);
	hideCursor(consolebuf[consolebuf.size() - 1]);
	SetConsoleActiveScreenBuffer(consolebuf[consolebuf.size() - 1]);
}

void Console::closeConsoleBuf()
{
	if (!consolebuf.empty())
	{
		CloseHandle(consolebuf[consolebuf.size() - 1]);
		consolebuf.pop_back();
	}
}

void Console::returnLastConsoleBuf()
{

}


void Console::hideCursor(HANDLE hOutputHandle)
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 }; 
	SetConsoleCursorInfo(hOutputHandle, &cursor_info);
}

void Console::showCursor(HANDLE hOutputHandle)
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 1 };
	SetConsoleCursorInfo(hOutputHandle, &cursor_info);
}

bool Console::isEmpty()
{
	return consolebuf.empty();
}

int Console::size()
{
	return consolebuf.size();
}


Console::~Console()
{
}
