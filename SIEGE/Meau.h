#pragma once

#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

#include "GameManager.h"
#include "DrawingBoard.h"

class Meau
{
public:
	Meau();

	void displayMeau();

	~Meau();
private:
	void displayBasic(HANDLE &, Graph &);

	void displayScore();
	void displayDraw();
	void displayPokedex();

	int testKey(HANDLE &, COORD &, COORD, int);
	int testKey(HANDLE &, COORD &,int);

	void choseMeau();
	void choseDraw();
	void chosePokedex();

	void displayHero();
	void displayBoss();
	void displayMonster();
	void displayGraph(COORD, Graph &, HANDLE &);


	void showArrow(HANDLE &, COORD);
	void hideArrow(HANDLE &, COORD);
	void showChose(HANDLE &, COORD);
	void hideChose(HANDLE &, COORD);

	COORD top;
	short offset;
	int choseUI;

	HANDLE MeauBuf;
	HANDLE ScoreBuf;
	HANDLE DrawBuf;
	HANDLE PokedexBuf;

	vector <string> meau;
	vector <string> draw;

	GameManager game;
	DrawingBoard drawingboard;
};

