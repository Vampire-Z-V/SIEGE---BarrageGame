#include "GameManager.h"
#include "Console.h"
#include "Resoure.h"

#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>

#define KEYDOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1:0)
#define KEYTEST_FRAME_PER_SECOND	30
#define GAME_FRAME_PER_SECOND		50

#define KEYTEST_SKIP_TICKS	( 1000 / KEYTEST_FRAME_PER_SECOND )
#define GAME_SKIP_TICKS		( 1000 / GAME_FRAME_PER_SECOND )

#define MONSTER_MOVE_FRAME_PER_SECOND 10
#define MONSTER_MOVE_SKIP_TICK ( 1000 / MONSTER_MOVE_FRAME_PER_SECOND )

#define HERO_MAX_SHOOTER 20
#define MONSTER_KILL 10

#define	STRONG 0

extern Console console;
extern Resoure resoure;
extern int choseHero;

long score;
vector <ImpactBox> impactRec;

GameManager::GameManager()
{
}

void GameManager::start()
{
	SetConsoleTitle("SIEGE V3.2 ----- GAIMGING");
	initialGame();
	createBuf();
	initialHero();
	score = 0;

	srand((unsigned)time(0));

	DWORD game_time_tick, keytest_time_tick;
	game_time_tick = keytest_time_tick = GetTickCount();
	while (gameStatus != GAMEOVER)
	{
		switch (gameStatus)
		{
			case GAMING:
				while (GetTickCount() > keytest_time_tick)
				{
					testKey();
					keytest_time_tick += KEYTEST_SKIP_TICKS;
				}
				while (GetTickCount() > game_time_tick)
				{
					updateALL();
					game_time_tick += GAME_SKIP_TICKS;
				}

				displayALL();
				break;
			case WIN:
				end();
				win();
				return;
		}
	}
	end();
	gameover();
}

void GameManager::clearBuf()
{
	DWORD dwNumberOfCharsWritten;

	FillConsoleOutputCharacter(outputCache, 0, coordBufSize.X * coordBufSize.Y, { 0,0 }, &dwNumberOfCharsWritten);
}

void GameManager::displayALL()
{
	clearBuf();

	if (isBoss)
		displayBoss();
	else
		displayMonster();
	displayBullet();
	displayHero();
	displayUI();

	if (HeroLife <= 0)
		gameStatus = GAMEOVER;

	DWORD dwNumberOfCharsWritten;
	char * buf = new char[coordBufSize.X * coordBufSize.Y];
	WORD * att = new WORD[coordBufSize.X * coordBufSize.Y];

	ReadConsoleOutputCharacter(outputCache, buf, coordBufSize.X * coordBufSize.Y, { 0,0 }, &dwNumberOfCharsWritten);
	ReadConsoleOutputAttribute(outputCache, att, coordBufSize.X * coordBufSize.Y, { 0,0 }, &dwNumberOfCharsWritten);
	WriteConsoleOutputAttribute(output, att, coordBufSize.X * coordBufSize.Y, { 0,0 }, &dwNumberOfCharsWritten);
	WriteConsoleOutputCharacter(output, buf, coordBufSize.X * coordBufSize.Y, { 0,0 }, &dwNumberOfCharsWritten);

	delete[] buf;
	delete[] att;
}

void GameManager::emptyShooter()
{
	while (!totalShooter.empty())
	{
		delete totalShooter[totalShooter.size() - 1];
		totalShooter.pop_back();
	}
}

void GameManager::gameover()
{
	SetConsoleTitle("SIEGE V3.2 ----- GAME OVER");
	HANDLE htemp;
	createBuf(htemp, resoure.getLoseGraph(), {13,2});
	displayScore({ 50,27 }, htemp);

	console.closeConsoleBuf();
}

void GameManager::win()
{
	SetConsoleTitle("SIEGE V3.2 ----- WIN");
	HANDLE htemp;
	createBuf(htemp, resoure.getWinGraph(), {0,1});
	displayScore({ 50,27 }, htemp);

	console.closeConsoleBuf();
}

void GameManager::displayScore(COORD startprint, HANDLE & hBuf)
{
	DWORD dwNumberOfCharsWritten;
	string print = "SCORE: ";

	string change;
	stringstream ss;
	ss << score;
	ss >> change;
	print += change;
	FillConsoleOutputAttribute(hBuf, FOREGROUND_GREEN | FOREGROUND_INTENSITY, print.size(), startprint, &dwNumberOfCharsWritten);
	WriteConsoleOutputCharacter(hBuf, &print[0], print.size(), startprint, &dwNumberOfCharsWritten);

	startprint.Y++;

	int i = 0;
	bool flag = false;
	for (; i < 15; i++)
	{
		if (score > resoure.getScore(i).score)
		{
			flag = true;
			break;
		}
	}

	if (flag)
	{
		FillConsoleOutputAttribute(hBuf, FOREGROUND_GREEN | FOREGROUND_INTENSITY, print.size(), startprint, &dwNumberOfCharsWritten);
		string name = "NAME: ";
		WriteConsoleOutputCharacter(hBuf, &name[0], name.size(), startprint, &dwNumberOfCharsWritten);

		startprint.X += 7;
		name.clear();

		SetConsoleCursorPosition(hBuf, startprint);
		CONSOLE_CURSOR_INFO cursor_info = { 20, 1 };
		SetConsoleCursorInfo(hBuf, &cursor_info);

		for (short i = 0;;)
		{
			SetConsoleCursorPosition(hBuf, { startprint.X + i, startprint.Y });

			int ch = _getch();

			if (isprint(ch) && name.size() <= 8)
			{
				FillConsoleOutputCharacter(hBuf, ch, 1, { startprint.X + i,startprint.Y }, &dwNumberOfCharsWritten);
				char record_ch = ch;
				name.push_back(record_ch);
				i++;
			}
			else if (ch == '\b')
			{
				if (i > 0)
				{
					FillConsoleOutputCharacter(hBuf, 0, 1, { startprint.X + i - 1,startprint.Y }, &dwNumberOfCharsWritten);
					name.pop_back();
					i--;
				}
			}
			else if (ch == '\r')
				break;
		}
		while (name.size() < 8)
			name.push_back(0);

		resoure.updateScore(i, name, score);
		cursor_info = { 1, 0 };
		SetConsoleCursorInfo(hBuf, &cursor_info);
	}

	while (true)
	{
		if (_kbhit())
		{
			int ch = _getch();
			if (ch == '\r')
				break;
		}
	}
}

void GameManager::end()
{
	emptyShooter();
	console.closeConsoleBuf();
	console.closeConsoleBuf();
}

void GameManager::displayUI()
{
	if (isBoss)
		displayBossHP();

	displayScore();
	displayHeroHP();
}

void GameManager::displayScore()
{
	COORD startprint = { 20,0 };
	if (isBoss)
		startprint.Y = 3;
	DWORD dwNumberOfCharsWritten;
	string print = "Score: ";

	string change;
	stringstream ss;
	ss << score;
	ss >> change;
	print += change;
	FillConsoleOutputAttribute(outputCache, FOREGROUND_GREEN | FOREGROUND_INTENSITY, print.size(), startprint, &dwNumberOfCharsWritten);
	WriteConsoleOutputCharacter(outputCache, &print[0], print.size(), startprint, &dwNumberOfCharsWritten);
}

void GameManager::displayHeroHP()
{


	COORD startprint = { 0,0 };
	if (isBoss)
		startprint.Y = 3;

	DWORD dwNumberOfCharsWritten;
	string print = "Hero's Life: ";
	string change;
	stringstream ss;
	ss << HeroLife;
	ss >> change;
	print += change;
	FillConsoleOutputAttribute(outputCache, FOREGROUND_GREEN | FOREGROUND_INTENSITY, print.size(), startprint, &dwNumberOfCharsWritten);
	WriteConsoleOutputCharacter(outputCache, &print[0], print.size(), startprint, &dwNumberOfCharsWritten);
}

void GameManager::displayBossHP()
{
	int HP = boss->getHP();
	int maxHP = boss->getMaxHp();
	if (HP <= 0)
	{
		gameStatus = WIN;
		boss->emptyShooter();
		delete boss;
		return;
	}

	DWORD dwNumberOfCharsWritten;
	WORD color;
	if (HP < maxHP / 10)
		color = FOREGROUND_RED | FOREGROUND_INTENSITY;
	else if (HP < maxHP / 3)
		color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	else
		color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

	for (short i = 0; i < ( maxHP + 1 ) / 2; i++)
	{
		if (i < ( HP + 1 ) / 2)
		{
			FillConsoleOutputAttribute(outputCache, color, 1, { i,0 }, &dwNumberOfCharsWritten);
			FillConsoleOutputCharacter(outputCache, '@', 1, { i,0 }, &dwNumberOfCharsWritten);
			FillConsoleOutputAttribute(outputCache, color, 1, { i,1 }, &dwNumberOfCharsWritten);
			FillConsoleOutputCharacter(outputCache, '@', 1, { i,1 }, &dwNumberOfCharsWritten);
		}
	}

	for (short i = 0; i < coordBufSize.X; i++)
	{
		FillConsoleOutputAttribute(outputCache, FOREGROUND_GREEN | FOREGROUND_INTENSITY, 1, { i,2 }, &dwNumberOfCharsWritten);
		FillConsoleOutputCharacter(outputCache, '-', 1, { i,2 }, &dwNumberOfCharsWritten);
	}
}

void GameManager::displayHero()
{
	if (isAlive)
		myHero.display(outputCache);
}

void GameManager::displayBoss()
{
	boss->display(outputCache);
}

void GameManager::displayMonster()
{
	for (auto i : monster)
	{
		i.display(outputCache);
	}
}

void GameManager::displayBullet()
{
	for (auto &i : totalShooter)
	{
		i->displayBullet(outputCache);
	}
	if (isBoss)
		boss->displayShooter();
}


GameManager::~GameManager()
{

}

void GameManager::initialGame()
{
	srWindowRect = { 0,0,199,39 };
	coordBufSize = { srWindowRect.Right + 1,srWindowRect.Bottom + 1 };
	gameStatus = GAMING;
	isBoss = false;

	monster.clear();
	totalShooter.clear();

	killMonsterCount = 0;
	HeroLife = 3;
	isAlive = false;

	b_UP = false;
	b_DOWN = false;
	b_LEFT = false;
	b_RIGHT = false;

	b_FIRE = false;
	b_ESCAPE = false;
	b_SLOW = false;

	increaseHeroLife_score = 1000 * 99;
}

void GameManager::createBuf()
{
	console.createNewConsoleBuf({ 9,12 }, srWindowRect);
	console.closeConsoleBuf();
	console.createNewConsoleBuf({ 9,12 }, srWindowRect);
	outputCache = console.getNowConsoleBuf();

	console.createNewConsoleBuf({ 9,12 }, srWindowRect);
	console.closeConsoleBuf();
	console.createNewConsoleBuf({ 9,12 }, srWindowRect);
	output = console.getNowConsoleBuf();

	impactRec.resize(coordBufSize.X * coordBufSize.Y);
}

void GameManager::createBuf(HANDLE & hBuf, Graph & graph, COORD offset)
{
	console.createNewConsoleBuf({ 9, 20 }, { 0,0,120,30 });
	console.closeConsoleBuf();
	console.createNewConsoleBuf({ 9, 20 }, { 0,0,120,30 });

	hBuf = console.getNowConsoleBuf();
	DWORD dwNumberOfCharsWritten;

	for (int count = 0; count < graph.the_num_of_ch; count++)
	{
		COORD drawPos = { graph.data[count].chPos.X+offset.X  , graph.data[count].chPos.Y+offset.Y };

		FillConsoleOutputAttribute(hBuf, graph.data[count].color, 1, drawPos, &dwNumberOfCharsWritten);
		FillConsoleOutputCharacter(hBuf, graph.data[count].ch, 1, drawPos, &dwNumberOfCharsWritten);
	}
}

void GameManager::initialHero()
{
	Hero temp(choseHero);
	temp.setHP(HeroLife);
	myHero = temp;
	isAlive = true;
	hero_shooter_count = 0;
}

void GameManager::initialBoss()
{
	isBoss = true;
	int photo = rand() % resoure.getBossGraphCount() + 1;
	boss = new Boss(outputCache, photo);
	boss->setHP(400);
}

void GameManager::getMonster(int chose)
{
	int randPos = rand();
	COORD appearPos;
	bool forward;
	int monstergraphcount = resoure.getMonsterGraphCount();
	int photo = rand() % monstergraphcount + 1;
	switch (chose)
	{
		case 0:
			{
				appearPos.Y = randPos % 30 + 5;
				if (randPos % 2)
				{
					appearPos.X = 0;
					forward = true;
				}
				else
				{
					appearPos.X = 199;
					forward = false;
				}

				for (auto &i : monster)
				{
					i.setHP(STRONG);
					i.setPosition(appearPos);
					i.setGoForward(forward);
					i.setGraph(photo);
				}
			}
			break;
		case 1:
			{
				appearPos.X = randPos % 200;
				if (randPos % 2)
				{
					appearPos.Y = 0;
					forward = true;
				}
				else
				{
					appearPos.Y = 39;
					forward = false;
				}
				for (auto &i : monster)
				{
					i.setHP(2);
					i.setPosition(appearPos);
					i.setGraph(photo);
					i.setGoForward(forward);
				}
			}
			break;
		case 2:
			{
				appearPos.X = 120;
				appearPos.Y = 0;
				for (int i = 0; i < 5; i++)
				{
					monster[i].setHP(4);
					monster[i].setPosX(appearPos.X + i * 12);
					monster[i].setPosY(appearPos.Y);
					monster[i].setGraph(photo);
					monster[i].setGoForward(true);
				}
				appearPos.X -= 6;
				appearPos.Y = 39;
				for (int i = 5; i < 10; i++)
				{
					monster[i].setHP(4);
					monster[i].setPosX(appearPos.X + ( i - 5 ) * 12);
					monster[i].setPosY(appearPos.Y);
					monster[i].setGraph(photo);
					monster[i].setGoForward(false);
				}
			}
			break;
		case 3:
			{
				randPos %= 3;
				appearPos = { 200,19 };
				monster[0].setHP(STRONG);
				monster[0].setPosition(appearPos);
				monster[0].setGraph(photo);
				monster[0].setGoForward(true);

				switch (randPos)
				{
					case 0:

						for (int i = 1; i < 5; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + i * 10);
							monster[i].setPosY(appearPos.Y - i * 4);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						for (int i = 5; i < 9; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + ( i - 4 ) * 10);
							monster[i].setPosY(appearPos.Y + ( i - 4 ) * 4);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						monster[9].setHP(STRONG);
						monster[9].setPosX(appearPos.X + 20);
						monster[9].setPosY(appearPos.Y);
						monster[9].setGraph(photo);
						monster[9].setGoForward(true);
						break;
					case 1:
						for (int i = 1; i < 6; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + i * 10);
							monster[i].setPosY(appearPos.Y);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						for (int i = 6; i < 8; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + 3 * 10);
							monster[i].setPosY(appearPos.Y + ( i - 5 ) * 5);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						for (int i = 8; i < 10; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + 3 * 10);
							monster[i].setPosY(appearPos.Y - ( i - 7 ) * 5);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						break;
					case 2:
						for (int i = 1; i < 4; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + i * 10);
							monster[i].setPosY(appearPos.Y);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						for (int i = 4; i < 6; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + ( i - 3 ) * 10);
							monster[i].setPosY(appearPos.Y + ( i - 3 ) * 7);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						for (int i = 6; i < 8; i++)
						{
							monster[i].setHP(STRONG);
							monster[i].setPosX(appearPos.X + ( i - 5 ) * 10);
							monster[i].setPosY(appearPos.Y - ( i - 5 ) * 7);
							monster[i].setGraph(photo);
							monster[i].setGoForward(true);
						}
						monster[8].setHP(STRONG);
						monster[8].setPosX(appearPos.X + 4 * 10);
						monster[8].setPosY(appearPos.Y - 3);
						monster[8].setGraph(photo);
						monster[8].setGoForward(true);

						monster[9].setHP(STRONG);
						monster[9].setPosX(appearPos.X + 4 * 10);
						monster[9].setPosY(appearPos.Y + 3);
						monster[9].setGraph(photo);
						monster[9].setGoForward(true);
						break;
				}
			}
			break;

		default:
			break;
	}
}

void GameManager::testKey()
{
	if (KEYDOWN(VK_UP) && ( !b_UP || !b_SLOW ))
	{
		myHero.increaseY(-1);
		b_UP = true;
	}
	if (KEYDOWN(VK_DOWN) && ( !b_DOWN || !b_SLOW ))
	{
		myHero.increaseY(1);
		b_DOWN = true;
	}
	if (KEYDOWN(VK_LEFT) && ( !b_LEFT || !b_SLOW ))
	{
		myHero.increaseX(-1);
		b_LEFT = true;
	}
	if (KEYDOWN(VK_RIGHT) && ( !b_RIGHT || !b_SLOW ))
	{
		myHero.increaseX(1);
		b_RIGHT = true;
	}
	if (KEYDOWN(VK_LCONTROL) && !b_SLOW)
	{
		b_SLOW = true;
	}
	if (KEYDOWN('Z') && t_fire == 0)
	{
		b_FIRE = true;
		t_fire = GetTickCount();
	}

	if (!KEYDOWN(VK_UP))		b_UP = false;
	if (!KEYDOWN(VK_DOWN))		b_DOWN = false;
	if (!KEYDOWN(VK_LEFT))		b_LEFT = false;
	if (!KEYDOWN(VK_RIGHT))		b_RIGHT = false;
	if (!KEYDOWN(VK_LCONTROL))	b_SLOW = false;
	if (!KEYDOWN('Z'))			b_FIRE = false;
}

void GameManager::clearImpactBox()
{
	string scoreRec;
	stringstream ss;
	ss << score;
	ss >> scoreRec;

	for (int i = 0; i < coordBufSize.X * coordBufSize.Y; i++)
	{
		if (isBoss)
		{
			for (; i < coordBufSize.X * 3 + 27 + scoreRec.size(); i++)
			{
				impactRec[i].isWall = true;
				impactRec[i].isHeroBullet = impactRec[i].isMonsterBullet = impactRec[i].isHero = impactRec[i].isMonster = false;
			}
		}
		else
		{
			for (; i < 27 + scoreRec.size(); i++)
			{
				impactRec[i].isWall = true;
				impactRec[i].isHeroBullet = impactRec[i].isMonsterBullet = impactRec[i].isHero = impactRec[i].isMonster = false;
			}
		}
		impactRec[i].isWall = impactRec[i].isHeroBullet = impactRec[i].isMonsterBullet = impactRec[i].isHero = impactRec[i].isMonster = false;
	}
}

void GameManager::updateALL()
{
	clearImpactBox();

	updateHero();

	if (killMonsterCount >= MONSTER_KILL && monster.empty())
	{
		initialBoss();
		killMonsterCount = 0;
	}
	if (isBoss)
		updateBoss();
	else
		updateMonster();

	updateBullet();

}

void GameManager::updateHero()
{
	if (HeroLife <= 0)
	{
		gameStatus = GAMEOVER;
		return;
	}
	if (!isAlive)
	{
		initialHero();
		if (isBoss)
			boss->emptyShooter();
		emptyShooter();
	}
	if (b_FIRE)
	{
		if (GetTickCount() > t_fire)
		{
			if (hero_shooter_count < HERO_MAX_SHOOTER)
			{
				Shooter *newshooter;
				newshooter = myHero.addShooter(NORMAL);
				totalShooter.push_back(newshooter);
				hero_shooter_count++;
			}
			t_fire += 1000 / 5;
		}
	}
	else
	{
		t_fire = 0;
	}
	myHero.update();
}

void GameManager::updateMonster()
{
	static int chose;
	static int shoot = 0;
	if (monster.size() == 0)
	{
		chose = rand() % 4;
		monster.resize(10);
		getMonster(chose);
		t_monstermove = GetTickCount();
		shoot = 20;
	}

	if (GetTickCount() > t_monstermove)
	{
		for (int i = monster.size() - 1; i > 0; i--)
		{
			if (abs(monster[i].getPosition().X - monster[i - 1].getPosition().X) >= 2 * ( monster[i - 1].getGraphCenterPos().X + 2 )
				|| abs(monster[i].getPosition().Y - monster[i - 1].getPosition().Y) >= 2 * ( monster[i - 1].getGraphCenterPos().Y + 2 ))
				monster[i].move(chose, false);
		}
		monster[0].move(chose, true);
		t_monstermove += MONSTER_MOVE_SKIP_TICK;
		shoot++;
	}


	for (auto i = monster.begin(); i != monster.end();)
	{
		COORD rec = i->getPosition();
		if (chose != 3 && ( rec.X < 0 || rec.X >= 200 || rec.Y < 0 || rec.Y >= 40 || impactRec[rec.Y * 200 + rec.X].isWall ))
			i = monster.erase(i);
		else if (chose == 3 && rec.X < 0)
			i = monster.erase(i);
		else
			i++;
	}

	for (auto i : monster)
	{
		i.update();
	}

	if (monster.size() > 0 && shoot < 5)
	{
		BulletStyle temp;
		switch (rand() % 3)
		{
			case 0: temp = NORMAL; break;
			case 1: temp = RADIAL; break;
			case 2: temp = SQUARE; break;
		}
		Shooter *newshooter = monster[rand() % monster.size()].addShooter(temp);
		totalShooter.push_back(newshooter);
	}
	if (shoot == 30)
		shoot = 0;
}

void GameManager::updateBoss()
{
	boss->update();
}

void GameManager::updateBullet()
{
	for (auto &i : totalShooter)
		i->moveBullet();
	if (isBoss)
		boss->updateShooter();

	for (int x = 0; x < coordBufSize.X; x++)
	{
		for (int y = 0; y < coordBufSize.Y; y++)
		{
			if (impactRec[y * 200 + x].isHeroBullet && impactRec[y * 200 + x].isMonster)
			{
				if (isBoss)
					boss->breakArmor();
				else
				{
					for (auto i = monster.begin(); i != monster.end();)
					{
						if (abs(x - i->getPosition().X) <= i->getGraphCenterPos().X + 1 && abs(y - i->getPosition().Y) <= i->getGraphCenterPos().Y + 1)
						{

							i->increaseHP(-1);
							if (i->getHP() <= 0)
							{
								killMonsterCount++;
								i = monster.erase(i);
							}
							break;
						}
						else
							i++;
					}
				}
				score += 99;
				if (score == increaseHeroLife_score)
				{
					HeroLife++;
					increaseHeroLife_score += 1000 * 99;
				}
			}
			if (impactRec[y * 200 + x].isHero && ( impactRec[y * 200 + x].isMonsterBullet || impactRec[y * 200 + x].isMonster ))
			{
				HeroLife--;
				isAlive = false;
			}
		}
	}

	for (auto i = totalShooter.begin(); i != totalShooter.end();)
	{
		( *i )->clearBullet();
		if (( *i )->getBulletCount() == 0)
		{
			if (!( *i )->getIsEnemy())
				hero_shooter_count--;
			delete ( *i );
			i = totalShooter.erase(i);
		}
		else
			i++;
	}
	if (isBoss)
		boss->clearShooter();
}
