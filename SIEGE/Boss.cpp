#include "Boss.h"

#include <cstdlib>
#include <cmath>

#define MOVE_FRAME_PER_SECOND 14
#define MOVE_SKIP_TICK ( 1000 / MOVE_FRAME_PER_SECOND )
#define WAIT_FRAME 60
#define ARMOR 0

extern Resoure resoure;
extern vector <ImpactBox> impactRec;

Boss::Boss(HANDLE outputhandle, int num)
{
	BossGraph = resoure.getBossGraph(num);
	Armor = ARMOR;
	COORD initPos = { 200,19 };
	setPosition(initPos);
	t_move = GetTickCount();
	t_wait = 0;
	SwitchDirection = false;
	outputbullet = outputhandle;
}

void Boss::breakArmor()
{
	Armor--;
	if (Armor <= 0)
	{
		Role::increaseHP(-1);
		Armor = ARMOR;
	}
}

void Boss::addShooter(COORD shootPos, bool fast)
{
	BulletStyle bullet_style;
	Shooter newshooter;
	if (fast)
	{
		bullet_style = SQUARE;
		newshooter.setBulletSpeed(20);
	}
	else
		switch (rand() % 4)
		{

			case 0:
				bullet_style = NORMAL;
				newshooter.setBulletSpeed(30);
				break;
			case 1:
				bullet_style = SPIRAL;
				newshooter.setBulletSpeed(30);
				break;
			case 2:
				bullet_style = RADIAL;
				newshooter.setBulletSpeed(20);
				break;
			case 3:
				bullet_style = SQUARE;
				newshooter.setBulletSpeed(12);
				break;
		}

	newshooter.setBulletStyle(bullet_style, true);
	newshooter.setBase(shootPos);
	newshooter.setBelong(true);
	newshooter.initialOthers();

	totalShooter.push_back(newshooter);
}

void Boss::shoot()
{
	COORD currentPos = Role::getPosition();
	short offset_1 = rand() % 7;
	short offset_2 = rand() % 7;
	if (abs(t_wait) % 10 == 0)
	{
		addShooter(currentPos);
	}
	else if (abs(t_wait) % 10 == 5)
	{
		addShooter({ currentPos.X, currentPos.Y + offset_1 });
		addShooter({ currentPos.X, currentPos.Y - offset_2 });
	}
	if (Role::getHP() < Role::getMaxHp() / 10 && abs(t_wait) % 15 == 0)
	{
		addShooter({ currentPos.X, currentPos.Y + 5 },true);
		addShooter({ currentPos.X, currentPos.Y - 5 },true);
		addShooter({ currentPos.X, currentPos.Y + 2 },true);
		addShooter({ currentPos.X, currentPos.Y - 2 },true);
	}
}

void Boss::updateShooter()
{
	for (auto &i : totalShooter)
		i.moveBullet();
}

void Boss::clearShooter()
{
	for (auto i = totalShooter.begin(); i != totalShooter.end();)
	{
		i->clearBullet();
		if (i->getBulletCount() == 0)
		{
			i = totalShooter.erase(i);
		}
		else
			i++;
	}
}

void Boss::emptyShooter()
{
	while (!totalShooter.empty())
		totalShooter.pop_back();
}

void Boss::displayShooter()
{
	for (auto i : totalShooter)
		i.displayBullet(outputbullet);
}

void Boss::update()
{
	move();
	COORD coordCenterPos = Role::getPosition();
	int offset_x = coordCenterPos.X - BossGraph.centerPos.X;
	int offset_y = coordCenterPos.Y - BossGraph.centerPos.Y;
	for (auto i : BossGraph.data)
	{
		if (i.chPos.X + offset_x >= 0 && i.chPos.X + offset_x < 200 && i.chPos.Y + offset_y >= 0 && i.chPos.Y + offset_y < 40)
			impactRec[( i.chPos.Y + offset_y ) * 200 + i.chPos.X + offset_x].isMonster = true;
	}
}

void Boss::move()
{
	COORD currentPos = Role::getPosition();
	if (GetTickCount() > t_move && currentPos.X > 120)
	{
		increaseX(-1);
		t_move += MOVE_SKIP_TICK;
	}
	else if (GetTickCount() > t_move)
	{
		if (currentPos.Y == 19)
		{
			shoot();
			if (abs(t_wait) >= 2 * WAIT_FRAME)
				t_wait = 0;
			else
				t_wait--;
		}
		if (!SwitchDirection && t_wait >= 0)
		{
			if (currentPos.Y <= BossGraph.centerPos.Y + 5)
			{
				shoot();
				t_wait++;
			}

			if (currentPos.Y > BossGraph.centerPos.Y + 5)
				increaseY(-1);
			else if (t_wait >= WAIT_FRAME)
			{
				SwitchDirection = !SwitchDirection;
				t_wait = 0;
			}
		}
		else if (SwitchDirection && t_wait >= 0)
		{
			if (currentPos.Y >= 35 - BossGraph.centerPos.Y)
			{
				shoot();
				t_wait++;
			}

			if (currentPos.Y < 35 - BossGraph.centerPos.Y)
				increaseY(1);
			else if (t_wait >= WAIT_FRAME)
			{
				SwitchDirection = !SwitchDirection;
				t_wait = 0;
			}
		}
		t_move += MOVE_SKIP_TICK * 2 ;
	}
}

void Boss::display(HANDLE & hOutputHandle)
{
	COORD coordDisplayPos = Role::getPosition();
	short offset_x = coordDisplayPos.X - BossGraph.centerPos.X;
	short offset_y = coordDisplayPos.Y - BossGraph.centerPos.Y;
	DWORD dwNumberOfCharsWritten;

	for (int count = 0; count < BossGraph.the_num_of_ch; count++)
	{
		if (BossGraph.data[count].chPos.X + offset_x >= 0 && BossGraph.data[count].chPos.Y + offset_y >= 0)
		{
			COORD drawPos = { BossGraph.data[count].chPos.X + offset_x , BossGraph.data[count].chPos.Y + offset_y };

			FillConsoleOutputAttribute(hOutputHandle, BossGraph.data[count].color, 1, drawPos, &dwNumberOfCharsWritten);
			FillConsoleOutputCharacterA(hOutputHandle, BossGraph.data[count].ch, 1, drawPos, &dwNumberOfCharsWritten);
		}
	}
}


Boss::~Boss()
{
}
