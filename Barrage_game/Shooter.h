#pragma once

#include <Windows.h>
#include <vector>
using namespace std;

#include "Resoure.h"

enum BulletStyle
{
	NORMAL,
	SPIRAL,
	RADIAL,
	SQUARE,
};


struct Bullet
{
	COORD bullet_pos;
	DWORD move_direction;
};


class Shooter
{
public:
	Shooter();

	void setBase(COORD cen_pos);
	void setBulletStyle(BulletStyle style, bool large = false);
	void setBulletSpeed(int speed);
	void setBelong(bool isEnemy);
	void initialOthers();

	COORD getBulletPosition(int i);
	int getBulletSpeed();
	bool getIsEnemy();
	int getBulletCount();
	
	void moveBullet();
	void clearBullet();
	void displayBullet(HANDLE &);


	~Shooter();

private:
	bool calcalateBulletPosition();
	
	int count;

	vector <Bullet> survival_bullet;
	BulletStyle bullet_style;
	int	bullet_speed;
	char bullet_character;
	DWORD bullet_color;
	bool isEnemy;
	bool isLarge;
	COORD centerPos;

	DWORD t_bulletspeed;
	DWORD t_bulletlayout;
	
	bool shoot;
	int layoutcount;
	int changedir;
};

