#include "Monster.h"

#include <cstdlib>

extern Resoure resoure;
extern vector <ImpactBox> impactRec;

Monster::Monster(int num)
{
	MonsterGraph = resoure.getMonsterGraph(num);

	changeflag = true;
}

void Monster::update()
{
	COORD coordCenterPos = Role::getPosition();
	int offset_x = coordCenterPos.X - MonsterGraph.centerPos.X;
	int offset_y = coordCenterPos.Y - MonsterGraph.centerPos.Y;
	for (auto i : MonsterGraph.data)
	{
		if (i.chPos.X + offset_x >= 0 && i.chPos.X + offset_x < 200 && i.chPos.Y + offset_y >= 0 && i.chPos.Y + offset_y < 40)
			impactRec[( i.chPos.Y + offset_y ) * 200 + i.chPos.X + offset_x].isMonster = true;
	}
}

Shooter * Monster::addShooter(BulletStyle bullet_style)
{
	COORD shootPos = Role::getPosition();
	shootPos.Y -= 2;
	shootPos.Y += rand() % 5;

	Shooter *newshooter = new Shooter;
	newshooter->setBulletStyle(bullet_style);
	newshooter->setBase(shootPos);
	newshooter->setBulletSpeed(10);
	newshooter->setBelong(true);
	newshooter->initialOthers();

	
	return newshooter;
}


COORD Monster::getGraphCenterPos()
{
	return MonsterGraph.centerPos;
}

void Monster::move(int select, bool first)
{
	static short directionRec[2];
	static COORD havegone = { 0,0 };
	switch (select)
	{
		case 0:
			if (goforward)
				Role::setPosX(Role::getPosition().X + 1);
			else
				Role::setPosX(Role::getPosition().X - 1);
			break;
		case 1:
			if (first)
			{
				if (changeflag)
				{
					if (goforward)
						directionRec[1] = 1;
					else
						directionRec[1] = -1;

					if (rand() % 2)
						directionRec[0] = 1;
					else
						directionRec[0] = -1;
					changeflag = false;
				}
			}
			Role::setPosX(Role::getPosition().X + directionRec[0]);
			Role::setPosY(Role::getPosition().Y + directionRec[1]);
			break;
		case 2:
			if (goforward)
				Role::setPosY(Role::getPosition().Y + 1);
			else
				Role::setPosY(Role::getPosition().Y - 1);
			break;
		case 3:
			Role::setPosX(Role::getPosition().X - 1);
			break;
		default:
			break;
	}
}

void Monster::setGoForward(bool forward)
{
	goforward = forward;
}

void Monster::setGraph(int num)
{
	MonsterGraph = resoure.getMonsterGraph(num);
}

void Monster::display(HANDLE & hOutputHandle)
{
	COORD coordDisplayPos = Role::getPosition();
	int offset_x = coordDisplayPos.X - MonsterGraph.centerPos.X;
	int offset_y = coordDisplayPos.Y - MonsterGraph.centerPos.Y;
	DWORD dwNumberOfCharsWritten;

	for (int count = 0; count < MonsterGraph.the_num_of_ch; count++)
	{
		if (MonsterGraph.data[count].chPos.X + offset_x >= 0 && MonsterGraph.data[count].chPos.Y + offset_y >= 0
			&& MonsterGraph.data[count].chPos.X + offset_x < 200 && MonsterGraph.data[count].chPos.Y + offset_y < 40)

		{
			COORD drawPos = { MonsterGraph.data[count].chPos.X + offset_x , MonsterGraph.data[count].chPos.Y + offset_y };

			FillConsoleOutputAttribute(hOutputHandle, MonsterGraph.data[count].color, 1, drawPos, &dwNumberOfCharsWritten);
			FillConsoleOutputCharacterA(hOutputHandle, MonsterGraph.data[count].ch, 1, drawPos, &dwNumberOfCharsWritten);
		}
	}
}


Monster::~Monster()
{
}
