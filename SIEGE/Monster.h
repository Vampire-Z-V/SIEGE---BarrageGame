#pragma once
#include "Resoure.h"
#include "Role.h"

class Monster :
	public Role
{
public:
	Monster(int = 1);

	void update();

	Shooter * addShooter(BulletStyle bullet_style);

	COORD getGraphCenterPos();

	void move(int select, bool first);
	void setGoForward(bool forward);
	void setGraph(int num);

	void display(HANDLE & hOutputHandle);
	
	virtual ~Monster();
private:
	Graph MonsterGraph;

	//move
	bool goforward;
	bool changeflag;
};

