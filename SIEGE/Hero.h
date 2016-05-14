#pragma once
#include "Role.h"
#include "Resoure.h"

class Hero :
	public Role
{
public:
	Hero(int = 1);
	Shooter * addShooter(BulletStyle bullet_style);

	void update();

	void display(HANDLE & hOutputHandle);
	virtual ~Hero();

private:
	Graph HeroGraph;
};

