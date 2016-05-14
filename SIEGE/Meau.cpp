#include "Meau.h"
#include "Console.h"

#include <iostream>
#include <conio.h>
#include <sstream>
using namespace std;

#define CONSOLEBUF_SIZE_WIDTH 120
#define CONSOLEBUF_SIZE_HEIGTH 30
#define FONT_SIZE_WIDTH 9
#define FONT_SIZE_HEIGHT 20

extern Console console;
extern Resoure resoure;
extern int choseHero;

Meau::Meau()
{
	top = { CONSOLEBUF_SIZE_WIDTH - 40 ,CONSOLEBUF_SIZE_HEIGTH - 20 };
	offset = 2;
	choseUI = 1;
	meau.clear();
	draw.clear();

	meau.push_back("START GAME");
	meau.push_back("DRAWING BOARD");
	meau.push_back("SCORE");
	meau.push_back("GALLERY");
	meau.push_back("EXIT GAME");

	draw.push_back("HERO");
	draw.push_back("MOSTER");
	draw.push_back("BOSS");

}

void Meau::displayMeau()
{
	displayBasic(MeauBuf, resoure.getMeauGraph(1));
	SetConsoleTitle("SIEGE V3.2");

	DWORD dwNumberOfCharsWritten;
	COORD start = top;
	start.X -= 23;
	start.Y++;
	for (int i = 0; i < meau.size(); i++)
	{
		WriteConsoleOutputCharacter(MeauBuf, meau[i].data(), meau[i].size(), start, &dwNumberOfCharsWritten);
		start.Y += offset;
	}

	choseMeau();
	console.closeConsoleBuf();
}


Meau::~Meau()
{
}

void Meau::displayBasic(HANDLE & hBuf, Graph & UI)
{
	console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });
	console.closeConsoleBuf();
	console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });

	hBuf = console.getNowConsoleBuf();
	DWORD dwNumberOfCharsWritten;

	for (int count = 0; count < UI.the_num_of_ch; count++)
	{
		COORD drawPos = { UI.data[count].chPos.X  , UI.data[count].chPos.Y };

		FillConsoleOutputAttribute(hBuf, UI.data[count].color, 1, drawPos, &dwNumberOfCharsWritten);
		FillConsoleOutputCharacter(hBuf, UI.data[count].ch, 1, drawPos, &dwNumberOfCharsWritten);
	}
}

void Meau::displayScore()
{
	displayBasic(ScoreBuf, resoure.getScoreGraph());
	SetConsoleTitle("SIEGE V3.2 ----- SCORE");

	_Score temp;
	stringstream ss;
	string score;
	DWORD dwNumberOfCharsWritten;
	WORD color;

	for (short i = 0; i < 15; i++)
	{
		temp = resoure.getScore(i);
		ss.clear();
		ss << temp.score;
		ss >> score;
		while (score.size() < 14)
			score.insert(0, " ");

		switch (i)
		{
			case 0:	
				color = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;	
				break;
			case 1:	
				color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;	
				break;
			case 2:	
				color = FOREGROUND_GREEN | FOREGROUND_RED;	
				break;
			default:
				color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
		}
		FillConsoleOutputAttribute(ScoreBuf, color, 26, { 60,i + 8 }, &dwNumberOfCharsWritten);
		WriteConsoleOutputCharacter(ScoreBuf, temp.name.data(), 8, { 60,i + 8 }, &dwNumberOfCharsWritten);
		WriteConsoleOutputCharacter(ScoreBuf, score.data(), 14, { 72,i + 8 }, &dwNumberOfCharsWritten);
	}

	while (true)
	{
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27)
				break;
		}
	}
	console.closeConsoleBuf();
	SetConsoleTitle("SIEGE V3.2");
}


void Meau::displayDraw()
{
	displayBasic(DrawBuf, resoure.getSubmeauGraph());
	SetConsoleTitle("SIEGE V3.2 ----- DRAWING BOARD");


	DWORD dwNumberOfCharsWritten;
	COORD start = top;
	start.X = 20;
	for (int i = 0; i < draw.size(); i++)
	{
		WriteConsoleOutputCharacter(DrawBuf, draw[i].data(), draw[i].size(), start, &dwNumberOfCharsWritten);
		start.Y += offset;
	}
	choseDraw();
	console.closeConsoleBuf();
	SetConsoleTitle("SIEGE V3.2");
}

void Meau::displayPokedex()
{
	displayBasic(PokedexBuf, resoure.getSubmeauGraph());
	SetConsoleTitle("SIEGE V3.2 ----- GALLERY");

	DWORD dwNumberOfCharsWritten;
	COORD start = top;
	start.X = 20;
	for (int i = 0; i < draw.size(); i++)
	{
		WriteConsoleOutputCharacter(PokedexBuf, draw[i].data(), draw[i].size(), start, &dwNumberOfCharsWritten);
		start.Y += offset;
	}
	chosePokedex();
	console.closeConsoleBuf();
	SetConsoleTitle("SIEGE V3.2");
}

int Meau::testKey(HANDLE & hBuf, COORD & position, COORD top, int item)
{
	DWORD dwNumberOfEventsRead;
	INPUT_RECORD inputRecord;

	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &inputRecord, 1, &dwNumberOfEventsRead);

	int returnValue = -1;
	switch (inputRecord.EventType)
	{
		case MOUSE_EVENT:
			if (inputRecord.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
			{
				for (short i = 0; i < item; i++)
					if (( inputRecord.Event.MouseEvent.dwMousePosition.Y == top.Y + offset * i )
						&& ( inputRecord.Event.MouseEvent.dwMousePosition.X >= position.X - 5 && inputRecord.Event.MouseEvent.dwMousePosition.X <= position.X + 15 ))
					{
						hideArrow(hBuf, position);
						position.Y = top.Y + offset * i;
						showArrow(hBuf, position);
						break;
					}
			}
			if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				if (( inputRecord.Event.MouseEvent.dwMousePosition.Y == position.Y )
					&& ( inputRecord.Event.MouseEvent.dwMousePosition.X >= position.X - 5 && inputRecord.Event.MouseEvent.dwMousePosition.X <= position.X + 15 ))
				returnValue = ( position.Y - top.Y ) / offset + 1;
			break;
		case KEY_EVENT:
			if (inputRecord.Event.KeyEvent.bKeyDown)
			{
				switch (inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
					case VK_DOWN:
						if (position.Y != top.Y + ( item - 1 ) * offset)
						{
							hideArrow(hBuf, position);
							position.Y += offset;
							showArrow(hBuf, position);
						}
						break;
					case VK_UP:
						if (position.Y != top.Y)
						{
							hideArrow(hBuf, position);
							position.Y -= offset;
							showArrow(hBuf, position);
						}
						break;
					case VK_RETURN:
						returnValue = ( position.Y - top.Y ) / offset + 1;
						break;
					case VK_ESCAPE:
						returnValue = 0;
						break;
				}
			}
			break;
	}
	return returnValue;
}

int Meau::testKey(HANDLE & hBuf, COORD & position,int item)
{
	DWORD dwNumberOfEventsRead;
	INPUT_RECORD inputRecord;
	COORD start = { 20,8 };

	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &inputRecord, 1, &dwNumberOfEventsRead);

	int returnValue = -1;
	switch (inputRecord.EventType)
	{
		case MOUSE_EVENT:
			if (inputRecord.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
			{
				for (short i = 0; i < item; i++)
				{
					if (i != 0 && i % 5 == 0)
					{
						start.X -= 100;
						start.Y += 8;
					}
					if (( inputRecord.Event.MouseEvent.dwMousePosition.Y > start.Y-5 && inputRecord.Event.MouseEvent.dwMousePosition.Y < start.Y + 5 )
						&& ( inputRecord.Event.MouseEvent.dwMousePosition.X > start.X+ i * 20 - 8 && inputRecord.Event.MouseEvent.dwMousePosition.X < start.X + i * 20 + 8 ))
					{
						hideChose(hBuf, position);
						position.X = start.X + i * 20;
						position.Y = start.Y;
						showChose(hBuf, position);
						break;
					}
				}
			}
			if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				choseHero = ( position.X - start.X ) / 20 + 5 * ( position.Y - start.Y ) / 8 + 1;
				returnValue = 0;
			}
			break;
		case KEY_EVENT:
			if (inputRecord.Event.KeyEvent.bKeyDown)
			{
				switch (inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
					case VK_ESCAPE:
						returnValue = 0;
						break;
				}
			}
			break;
	}
	return returnValue;
}

void Meau::choseMeau()
{
	int chose;
	COORD start = top;
	start.X -= 23;
	start.Y++;
	COORD rec = start;
	bool exit = false;

	showArrow(MeauBuf, start);
	while (!exit)
	{
		chose = testKey(MeauBuf, start, rec, meau.size());
		switch (chose)
		{
			case 1:
				game.start();
				console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });
				console.closeConsoleBuf();
				SetConsoleTitle("SIEGE V3.2");
				break;
			case 2:
				displayDraw();
				break;
			case 3:
				displayScore();
				break;
			case 4:
				displayPokedex();
				break;
			case 5:
				exit = true;
				break;
		}
	}
}


void Meau::choseDraw()
{
	int chose;
	COORD start = top;
	start.X = 20;
	bool exit = false;

	showArrow(DrawBuf, start);
	while (!exit)
	{
		chose = testKey(DrawBuf, start, top, draw.size());
		switch (chose)
		{
			case 0:
				exit = true;
				break;
			case 1:
				SetConsoleTitle("SIEGE V3.2 ----- DRAWING HERO");
				drawingboard.draw(HERO);
				console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });
				console.closeConsoleBuf();
				break;
			case 2:
				SetConsoleTitle("SIEGE V3.2 ----- DRAWING MONSTER");
				drawingboard.draw(MONSTER);
				console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });
				console.closeConsoleBuf();
				break;
			case 3:
				SetConsoleTitle("SIEGE V3.2 ----- DRAWING BOSS");
				drawingboard.draw(BOSS);
				console.createNewConsoleBuf({ FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT }, { 0,0,CONSOLEBUF_SIZE_WIDTH,CONSOLEBUF_SIZE_HEIGTH });
				console.closeConsoleBuf();
				break;
		}
		SetConsoleTitle("SIEGE V3.2 ----- DRAWING BOARD");
	}
}

void Meau::chosePokedex()
{
	int chose;
	COORD start = top;
	start.X = 20;
	bool exit = false;

	showArrow(PokedexBuf, start);
	while (!exit)
	{
		chose = testKey(PokedexBuf, start, top, draw.size());
		switch (chose)
		{
			case 0:
				exit = true;
				break;
			case 1:
				displayHero();

				break;
			case 2:
				displayMonster();

				break;
			case 3:
				displayBoss();

				break;

		}
	}
}

void Meau::displayHero()
{
	HANDLE htemp;
	displayBasic(htemp, resoure.getUIGraph());

	int count = resoure.getHeroGraphCount();
	COORD start = { 20,8 };

	for (short i = 0; i < count; i++)
	{
		if (i != 0 && i % 5 == 0)
		{
			start.X -= 100;
			start.Y += 8;
		}
		displayGraph({ start.X + i * 20,start.Y }, resoure.getHeroGraph(i + 1), htemp);
	}

	while (true)
	{
		int x=testKey(htemp, start, count);
		if (x == 0)
			break;
	}

	console.closeConsoleBuf();
}

void Meau::displayBoss()
{
	HANDLE htemp;
	displayBasic(htemp, resoure.getUIGraph());


	int count = resoure.getBossGraphCount();
	COORD start = { 25,9 };

	for (short i = 0; i < count; i++)
	{
		if (i != 0 && i % 3 == 0)
		{
			start.X -= 105;
			start.Y += 13;
		}
		displayGraph({ start.X + i * 35,start.Y }, resoure.getBossGraph(i + 1), htemp);
	}

	while (true)
	{
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27)
				break;
		}
	}
	console.closeConsoleBuf();
}

void Meau::displayMonster()
{
	HANDLE htemp;
	displayBasic(htemp, resoure.getUIGraph());


	int count = resoure.getMonsterGraphCount();
	COORD start = { 20,6 };

	for (short i = 0; i < count; i++)
	{
		if (i != 0 && i % 5 == 0)
		{
			start.X -= 100;
			start.Y += 8;
		}
		displayGraph({ start.X + i * 20,start.Y }, resoure.getMonsterGraph(i + 1), htemp);
	}

	while (true)
	{
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27)
				break;
		}
	}
	console.closeConsoleBuf();
}

void Meau::showArrow(HANDLE & hBuf, COORD position)
{
	DWORD dwNumberOfCharsWritten;
	FillConsoleOutputCharacter(hBuf, '>', 3, { position.X - 5,position.Y }, &dwNumberOfCharsWritten);
}

void Meau::hideArrow(HANDLE & hBuf, COORD position)
{
	DWORD dwNumberOfCharsWritten;
	FillConsoleOutputCharacter(hBuf, 0, 3, { position.X - 5,position.Y }, &dwNumberOfCharsWritten);
}

void Meau::showChose(HANDLE & hBuf, COORD position)
{
	DWORD dwNumberOfCharsWritten;
	FillConsoleOutputCharacter(hBuf, '@', 3, { position.X-1,position.Y+4 }, &dwNumberOfCharsWritten);
}

void Meau::hideChose(HANDLE & hBuf, COORD position)
{
	DWORD dwNumberOfCharsWritten;
	FillConsoleOutputCharacter(hBuf, 0, 3, { position.X-1,position.Y + 4 }, &dwNumberOfCharsWritten);
}

void Meau::displayGraph(COORD cenPos, Graph & graph, HANDLE & hBuf)
{
	int offset_x = cenPos.X - graph.centerPos.X;
	int offset_y = cenPos.Y - graph.centerPos.Y;

	COORD displayPos = cenPos;

	DWORD rec;

	for (int count = 0; count < graph.the_num_of_ch; count++)
	{
		if (graph.data[count].chPos.X + offset_x >= 0 && graph.data[count].chPos.Y + offset_y >= 0)
		{
			COORD drawPos = { graph.data[count].chPos.X + offset_x , graph.data[count].chPos.Y + offset_y };
			FillConsoleOutputAttribute(hBuf, graph.data[count].color, 1, drawPos, &rec);
			FillConsoleOutputCharacter(hBuf, graph.data[count].ch, 1, drawPos, &rec);
		}
	}
}