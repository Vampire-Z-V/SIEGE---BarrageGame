#include "Hero.h"

#include <iostream>
#include <fstream>
using namespace std;


extern Resoure resoure;
extern vector <ImpactBox> impactRec;

Hero::Hero(int num):Role({ 20,20 })
{
	HeroGraph = resoure.getHeroGraph(num);
}

Shooter * Hero::addShooter(BulletStyle bullet_style)
{
	Shooter *newshooter = new Shooter;
	newshooter->setBulletStyle(bullet_style);
	newshooter->setBase(Role::getPosition());
	newshooter->setBulletSpeed(50);
	newshooter->setBelong(false);
	newshooter->initialOthers();

	return newshooter;
}


void Hero::update()
{
	COORD center_pos = Role::getPosition();
	impactRec[center_pos.Y * 200 + center_pos.X].isHero = true;
}

void Hero::display(HANDLE & hOutputHandle)
{
	COORD coordDisplayPos = Role::getPosition();
	short offset_x = coordDisplayPos.X - HeroGraph.centerPos.X;
	short offset_y = coordDisplayPos.Y - HeroGraph.centerPos.Y;
	DWORD dwNumberOfCharsWritten;

	for (int count = 0; count < HeroGraph.the_num_of_ch; count++)
	{
		if (HeroGraph.data[count].chPos.X + offset_x >= 0 && HeroGraph.data[count].chPos.Y + offset_y >= 0
			&& HeroGraph.data[count].chPos.X + offset_x < 200 && HeroGraph.data[count].chPos.Y + offset_y < 40
			&& !( impactRec[200 * ( HeroGraph.data[count].chPos.Y + offset_y ) + ( HeroGraph.data[count].chPos.X + offset_x )].isWall ))

		{
			COORD drawPos = { HeroGraph.data[count].chPos.X + offset_x , HeroGraph.data[count].chPos.Y + offset_y };

			FillConsoleOutputAttribute(hOutputHandle, HeroGraph.data[count].color, 1, drawPos, &dwNumberOfCharsWritten);
			FillConsoleOutputCharacter(hOutputHandle, HeroGraph.data[count].ch, 1, drawPos, &dwNumberOfCharsWritten);
		}
	}
}

Hero::~Hero()
{
}
