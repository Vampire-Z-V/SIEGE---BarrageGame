#pragma once

#include <Windows.h>
#include <vector>
#include <cstdlib>
using namespace std;


class Console
{
public:
	Console();

	HANDLE getNowConsoleBuf();

	void createNewConsoleBuf(COORD cdFontSize, SMALL_RECT srWindowRect);
	void closeConsoleBuf();
	void returnLastConsoleBuf();

	void hideCursor(HANDLE hOutputHandle);
	void showCursor(HANDLE hOutputHandle);

	bool isEmpty();
	int size();

	~Console();

private:
	vector <HANDLE> consolebuf;
	CONSOLE_FONT_INFOEX fontinfoex;

};

