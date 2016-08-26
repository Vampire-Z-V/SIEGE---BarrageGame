#include "Role.h"
#include "Resoure.h"

extern vector <ImpactBox> impactRec;

Role::Role()
{
}

Role::Role(COORD initPos)
{
	Pos = initPos;
}

void Role::setPosition(COORD coordPos)
{
	Pos = coordPos;
}

void Role::setPosX(SHORT x)
{
	Pos.X = x;
}

void Role::setPosY(SHORT y)
{
	Pos.Y = y;
}

void Role::increaseX(short i)
{
	Pos.X += i;
	if (Pos.X < 0 || Pos.X >= 200 || impactRec[200 * Pos.Y + Pos.X].isWall)
		Pos.X -= i;
}

void Role::increaseY(short i)
{
	Pos.Y += i;
	if (Pos.Y < 0 || Pos.Y >= 40 || impactRec[200 * Pos.Y + Pos.X].isWall)
		Pos.Y -= i;
}

void Role::setHP(short max_hp)
{
	r_hp = r_maxhp = max_hp;
}

void Role::increaseHP(short hp)
{
	r_hp += hp;
}

int Role::getHP()
{
	return r_hp;
}

int Role::getMaxHp()
{
	return r_maxhp;
}

COORD Role::getPosition()
{
	return Pos;
}

Role::~Role()
{
}
