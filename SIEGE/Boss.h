#pragma once
#include "Resoure.h"
#include "Monster.h"

class Boss :
	public Monster
{
public:
	Boss(HANDLE outputhandle, int = 1);

	void breakArmor();

	void updateShooter();
	void clearShooter();
	void emptyShooter();
	void displayShooter();

	void update();
	void display(HANDLE & hOutputHandle);

	virtual ~Boss();
private:
	void addShooter(COORD shootPos, bool fast = false);
	void shoot();
	void move();

	Graph BossGraph;
	int Armor;
	vector <Shooter> totalShooter;
	DWORD t_move;

	HANDLE outputbullet;

	bool SwitchDirection;
	int t_wait;
};

