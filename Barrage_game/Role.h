#pragma once

#include <windows.h>
#include <vector>
using namespace std;

#include "Shooter.h"

class Role
{
public:
	Role();
	Role(COORD initPos);

	//Position
	void setPosition(COORD coordPos);
	void setPosX(SHORT x);
	void setPosY(SHORT y);
	void increaseX(short i);
	void increaseY(short i);

	void setHP(short max_hp);
	void increaseHP(short hp);

	int getHP();
	int getMaxHp();
	COORD getPosition();

	virtual void update() = 0;
	virtual void display(HANDLE & hOutputHandle) = 0;
	virtual ~Role();
private:
	short r_hp;
	int r_maxhp;

	COORD Pos;
};

