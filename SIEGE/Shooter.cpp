#include "Shooter.h"

#include<iostream>
using namespace std;
#define UP		0x0001
#define RIGHT	0x0002
#define DOWN	0x0004
#define	LEFT	0x0008

#define LAYOUT_TIME_SKIP_TICK (1000 / 5)

extern vector <ImpactBox> impactRec;
Shooter::Shooter()
{
	count = 1;
	t_bulletlayout = GetTickCount();
	shoot = false;
}

void Shooter::setBase(COORD cen_pos)
{
	centerPos = cen_pos;
}

void Shooter::setBulletStyle(BulletStyle style, bool large)
{
	isLarge = large;
	switch (style)
	{
		case NORMAL:
			if (!large)
				survival_bullet.resize(3);
			else
				survival_bullet.resize(10);
			break;
		case SPIRAL:
			survival_bullet.resize(80);
			break;
		case RADIAL:
			if (!large)
				survival_bullet.resize(40);
			else
				survival_bullet.resize(80);
			break;
		case SQUARE:
			survival_bullet.resize(24);
			break;
	}
	bullet_style = style;

}

void Shooter::setBulletSpeed(int speed)
{
	bullet_speed = speed;
}

void Shooter::setBelong(bool isEnemy)
{
	this->isEnemy = isEnemy;
}

void Shooter::initialOthers()
{
	switch (bullet_style)
	{
		case NORMAL:
			bullet_character = 'o';
			for (int i = 0; i < survival_bullet.size(); i++)
			{
				if (isEnemy)
					survival_bullet[i].move_direction = LEFT;
				else
					survival_bullet[i].move_direction = RIGHT;

				survival_bullet[i].bullet_pos = centerPos;
			}
			shoot = true;
			t_bulletspeed = GetTickCount();
			break;
		case SPIRAL:
			bullet_character = '@';
			for (int i = 0; i < survival_bullet.size(); i++)
			{
				switch (i % 16)
				{
					case 0: case 1:
						survival_bullet[i].move_direction = DOWN;
						break;
					case 2: case 3:
						survival_bullet[i].move_direction = DOWN | LEFT;
						break;
					case 4: case 5:
						survival_bullet[i].move_direction = LEFT;
						break;
					case 6: case 7:
						survival_bullet[i].move_direction = UP | LEFT;
						break;
					case 8: case 9:
						survival_bullet[i].move_direction = UP;
						break;
					case 10: case 11:
						survival_bullet[i].move_direction = UP | RIGHT;
						break;
					case 12: case 13:
						survival_bullet[i].move_direction = RIGHT;
						break;
					case 14: case 15:
						survival_bullet[i].move_direction = DOWN | RIGHT;
						break;
				}
				survival_bullet[i].bullet_pos = centerPos;
			}
			shoot = true;
			t_bulletspeed = GetTickCount();
			break;
		case RADIAL:
			bullet_character = '*';
			for (int i = 0; i < survival_bullet.size(); i++)
			{
				switch (i % 8)
				{
					case 0:
						survival_bullet[i].move_direction = DOWN;
						survival_bullet[i].bullet_pos.X = centerPos.X, survival_bullet[i].bullet_pos.Y = centerPos.Y + 1;
						break;
					case 1:
						survival_bullet[i].move_direction = DOWN | LEFT;
						survival_bullet[i].bullet_pos.X = centerPos.X - 2, survival_bullet[i].bullet_pos.Y = centerPos.Y + 1;
						break;
					case 2:
						survival_bullet[i].move_direction = LEFT;
						survival_bullet[i].bullet_pos.X = centerPos.X - 2, survival_bullet[i].bullet_pos.Y = centerPos.Y;
						break;
					case 3:
						survival_bullet[i].move_direction = UP | LEFT;
						survival_bullet[i].bullet_pos.X = centerPos.X - 2, survival_bullet[i].bullet_pos.Y = centerPos.Y - 1;
						break;
					case 4:
						survival_bullet[i].move_direction = UP;
						survival_bullet[i].bullet_pos.X = centerPos.X, survival_bullet[i].bullet_pos.Y = centerPos.Y - 1;
						break;
					case 5:
						survival_bullet[i].move_direction = UP | RIGHT;
						survival_bullet[i].bullet_pos.X = centerPos.X + 2, survival_bullet[i].bullet_pos.Y = centerPos.Y - 1;
						break;
					case 6:
						survival_bullet[i].move_direction = RIGHT;
						survival_bullet[i].bullet_pos.X = centerPos.X + 2, survival_bullet[i].bullet_pos.Y = centerPos.Y;
						break;
					case 7:
						survival_bullet[i].move_direction = DOWN | RIGHT;
						survival_bullet[i].bullet_pos.X = centerPos.X + 2, survival_bullet[i].bullet_pos.Y = centerPos.Y + 1;
						break;
				}
			}
			if (!isLarge)
			{
				layoutcount = 5;
			}
			else
			{
				layoutcount = 9;
			}
			break;
		case SQUARE:
			bullet_character = '#';

			for (int i = 0; i < survival_bullet.size(); i++)
			{
				if (i < 9)
				{
					survival_bullet[i].move_direction = UP | LEFT;
					survival_bullet[i].bullet_pos.X = centerPos.X - 4, survival_bullet[i].bullet_pos.Y = centerPos.Y - 2;
				}
				else if (i > 14)
				{
					survival_bullet[i].move_direction = DOWN | RIGHT;
					survival_bullet[i].bullet_pos.X = centerPos.X + 4, survival_bullet[i].bullet_pos.Y = centerPos.Y + 2;
				}
				else
				{
					switch (i)
					{
						case 9:
							survival_bullet[i].move_direction = UP | RIGHT;
							survival_bullet[i].bullet_pos.X = centerPos.X, survival_bullet[i].bullet_pos.Y = centerPos.Y - 2;
							break;
						case 10:
							survival_bullet[i].move_direction = UP | RIGHT;
							survival_bullet[i].bullet_pos.X = centerPos.X + 4, survival_bullet[i].bullet_pos.Y = centerPos.Y - 2;
							break;
						case 11:
							survival_bullet[i].move_direction = UP | RIGHT;
							survival_bullet[i].bullet_pos.X = centerPos.X + 4, survival_bullet[i].bullet_pos.Y = centerPos.Y;
							break;
						case 12:
							survival_bullet[i].move_direction = DOWN | LEFT;
							survival_bullet[i].bullet_pos.X = centerPos.X, survival_bullet[i].bullet_pos.Y = centerPos.Y + 2;
							break;
						case 13:
							survival_bullet[i].move_direction = DOWN | LEFT;
							survival_bullet[i].bullet_pos.X = centerPos.X - 4, survival_bullet[i].bullet_pos.Y = centerPos.Y + 2;
							break;
						case 14:
							survival_bullet[i].move_direction = DOWN | LEFT;
							survival_bullet[i].bullet_pos.X = centerPos.X - 4, survival_bullet[i].bullet_pos.Y = centerPos.Y;
							break;
					}
				}
			}
			layoutcount = 4;
			changedir = 0;
			break;
	}

	if (isEnemy)
		switch (bullet_style)
		{
			case NORMAL:
				bullet_color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			case SPIRAL:
				bullet_color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			case RADIAL:
				bullet_color =  FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			case SQUARE:
				bullet_color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
		}
	else
		bullet_color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
}

COORD Shooter::getBulletPosition(int i)
{
	return survival_bullet[i].bullet_pos;
}

int Shooter::getBulletSpeed()
{
	return bullet_speed;
}

bool Shooter::getIsEnemy()
{
	return isEnemy;
}

int Shooter::getBulletCount()
{
	return survival_bullet.size();
}

void Shooter::moveBullet()
{
	if (calcalateBulletPosition())
		for (auto Iter = survival_bullet.begin(); Iter != survival_bullet.end();)
		{
			if (Iter->bullet_pos.X < 0 || Iter->bullet_pos.X >= 200 || Iter->bullet_pos.Y < 0 || Iter->bullet_pos.Y >= 40 || impactRec[Iter->bullet_pos.Y * 200 + Iter->bullet_pos.X].isWall)
				Iter = survival_bullet.erase(Iter);
			else
			{
				if (isEnemy)
					impactRec[Iter->bullet_pos.Y * 200 + Iter->bullet_pos.X].isMonsterBullet = true;
				else
					impactRec[Iter->bullet_pos.Y * 200 + Iter->bullet_pos.X].isHeroBullet = true;
				Iter++;
			}
		}
}

void Shooter::clearBullet()
{
	for (auto Iter = survival_bullet.begin(); Iter != survival_bullet.end();)
	{
		if (Iter->bullet_pos.X < 0 || Iter->bullet_pos.X >= 200 || Iter->bullet_pos.Y < 0 || Iter->bullet_pos.Y >= 40 || ( !isEnemy&& impactRec[Iter->bullet_pos.Y * 200 + Iter->bullet_pos.X].isMonster ))
			Iter = survival_bullet.erase(Iter);
		else
		{
			Iter++;
		}
	}
}

void Shooter::displayBullet(HANDLE & hOutputHandle)
{
	DWORD dwNumberOfCharsWritten;
	for (auto i : survival_bullet)
	{
		FillConsoleOutputAttribute(hOutputHandle, bullet_color, 1, i.bullet_pos, &dwNumberOfCharsWritten);
		FillConsoleOutputCharacter(hOutputHandle, bullet_character, 1, i.bullet_pos, &dwNumberOfCharsWritten);
	}
}


Shooter::~Shooter()
{
}

bool Shooter::calcalateBulletPosition()
{
	if (!shoot && GetTickCount() > t_bulletlayout)
	{
		switch (bullet_style)
		{
			case RADIAL:
				for (int i = survival_bullet.size() - 1; i >= 0; i--)
				{
					if (i >= 8)
					{
						if (survival_bullet[i].bullet_pos.X != survival_bullet[i - 8].bullet_pos.X || survival_bullet[i].bullet_pos.Y != survival_bullet[i - 8].bullet_pos.Y)
						{

							survival_bullet[i].move_direction & UP ? survival_bullet[i].bullet_pos.Y-- : 0;
							survival_bullet[i].move_direction & DOWN ? survival_bullet[i].bullet_pos.Y++ : 0;
							survival_bullet[i].move_direction & RIGHT ? survival_bullet[i].bullet_pos.X += 2 : 0;
							survival_bullet[i].move_direction & LEFT ? survival_bullet[i].bullet_pos.X -= 2 : 0;
						}
					}
					else if (i < 8)
					{
						survival_bullet[i].move_direction & UP ? survival_bullet[i].bullet_pos.Y-- : 0;
						survival_bullet[i].move_direction & DOWN ? survival_bullet[i].bullet_pos.Y++ : 0;
						survival_bullet[i].move_direction & RIGHT ? survival_bullet[i].bullet_pos.X += 2 : 0;
						survival_bullet[i].move_direction & LEFT ? survival_bullet[i].bullet_pos.X -= 2 : 0;
					}
				}
				layoutcount--;
				if (layoutcount == 0)
				{
					shoot = true;
					t_bulletspeed = GetTickCount();
				}
				break;
			case SQUARE:
				changedir++;
				if ((int)survival_bullet.size() - 1 - changedir >= 0)
					survival_bullet[changedir].move_direction = survival_bullet[survival_bullet.size() - 1 - changedir].move_direction = RIGHT | UP;

				changedir++;
				if ((int)survival_bullet.size() - 1 - changedir >= 0)
					survival_bullet[changedir].move_direction = survival_bullet[survival_bullet.size() - 1 - changedir].move_direction = LEFT | DOWN;
				for (int i = survival_bullet.size() - 1; i >= 0; i--)
				{
					survival_bullet[i].move_direction & UP ? survival_bullet[i].bullet_pos.Y-- : 0;
					survival_bullet[i].move_direction & DOWN ? survival_bullet[i].bullet_pos.Y++ : 0;
					survival_bullet[i].move_direction & RIGHT ? survival_bullet[i].bullet_pos.X += 2 : 0;
					survival_bullet[i].move_direction & LEFT ? survival_bullet[i].bullet_pos.X -= 2 : 0;
				}

				layoutcount--;
				if (layoutcount <= 0)
				{
					for (int i = survival_bullet.size() - 1; i >= 0; i--)
					{
						if (i > 0 && i < 9)
						{
							if (i % 2)
								survival_bullet[i].move_direction = UP;
							else
								survival_bullet[i].move_direction = LEFT;
						}
						else if (i == 9)
						{
							survival_bullet[i].move_direction = UP;
						}
						else if (i == 11)
						{
							survival_bullet[i].move_direction = RIGHT;
						}
						else if (i == 12)
						{
							survival_bullet[i].move_direction = DOWN;
						}
						else if (i == 14)
						{
							survival_bullet[i].move_direction = LEFT;
						}
						else if (i > 14 && i < 23)
						{
							if (i % 2)
								survival_bullet[i].move_direction = DOWN;
							else
								survival_bullet[i].move_direction = RIGHT;
						}
					}
					shoot = true;
					t_bulletspeed = GetTickCount();
				}
				break;
		}
		t_bulletlayout += LAYOUT_TIME_SKIP_TICK;
		return true;
	}
	else if (shoot && GetTickCount() > t_bulletspeed)
	{
		for (int i = survival_bullet.size() - 1; i >= 0; i--)
		{
			if (i > 0)
			{
				if (survival_bullet[i].bullet_pos.X != survival_bullet[i - 1].bullet_pos.X || survival_bullet[i].bullet_pos.Y != survival_bullet[i - 1].bullet_pos.Y)
				{
					if (survival_bullet[i].move_direction & RIGHT)
					{
						if (survival_bullet[i].move_direction & UP)
						{
							survival_bullet[i].bullet_pos.X += 2;
							survival_bullet[i].bullet_pos.Y--;
							continue;
						}
						else if (survival_bullet[i].move_direction & DOWN)
						{
							survival_bullet[i].bullet_pos.X += 2;
							survival_bullet[i].bullet_pos.Y++;
							continue;
						}
						survival_bullet[i].bullet_pos.X++;
						continue;
					}
					else if (survival_bullet[i].move_direction & LEFT)
					{
						if (survival_bullet[i].move_direction & UP)
						{
							survival_bullet[i].bullet_pos.X -= 2;
							survival_bullet[i].bullet_pos.Y--;
							continue;
						}
						else if (survival_bullet[i].move_direction & DOWN)
						{
							survival_bullet[i].bullet_pos.X -= 2;
							survival_bullet[i].bullet_pos.Y++;
							continue;
						}
						survival_bullet[i].bullet_pos.X--;
						continue;
					}

					survival_bullet[i].move_direction & UP ? survival_bullet[i].bullet_pos.Y-- : 0;
					survival_bullet[i].move_direction & DOWN ? survival_bullet[i].bullet_pos.Y++ : 0;

				}
			}
			else
			{
				survival_bullet[i].move_direction & UP ? survival_bullet[i].bullet_pos.Y-- : 0;
				survival_bullet[i].move_direction & DOWN ? survival_bullet[i].bullet_pos.Y++ : 0;
				survival_bullet[i].move_direction & RIGHT ? survival_bullet[i].bullet_pos.X++ : 0;
				survival_bullet[i].move_direction & LEFT ? survival_bullet[i].bullet_pos.X-- : 0;
			}
		}
		t_bulletspeed += 1000 / bullet_speed;
		return true;
	}
	return false;
}
