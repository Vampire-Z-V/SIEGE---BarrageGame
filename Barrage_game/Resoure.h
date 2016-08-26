#pragma once

#include <Windows.h>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct ImpactBox
{
	bool isHero = false;
	bool isMonster = false;
	bool isHeroBullet = false;
	bool isMonsterBullet = false;
	bool isWall = false;
};

struct record
{
	COORD chPos;
	char ch;
	WORD color;
};

struct Graph
{
	COORD size;
	COORD centerPos;
	int the_num_of_ch;
	vector <record> data;
};

struct _Score
{
	string name;
	long score;
};

class Resoure
{
public:
	Resoure();

	void reset();
	void resetHero();
	void resetBoss();
	void resetMonster();
	void resetMeau();

	Graph getHeroGraph(int num);
	Graph getBossGraph(int num);
	Graph getMonsterGraph(int num);
	Graph getMeauGraph(int num);
	Graph getSubmeauGraph();
	Graph getScoreGraph();
	Graph getWinGraph();
	Graph getLoseGraph();
	Graph getUIGraph();

	_Score getScore(int num);
	void updateScore(int num, string & name, long score);

	int getHeroGraphCount();
	int getBossGraphCount();
	int getMonsterGraphCount();
	int getMeauGraphCount();

	~Resoure();
private:
	void setHeroGraph();
	void setBossGraph();
	void setMonsterGraph();
	void setMeauGraph();

	void setSubmeauGraph();
	void setScoreGraph();
	void setWinGraph();
	void setLoseGraph();
	void setUIGraph();

	void readRecord();

	string rf;
	string cl;
	stringstream ss;

	vector <Graph> Hero;
	vector <Graph> Boss;
	vector <Graph> Monster;
	vector <Graph> Meau;
	Graph Submeau;
	Graph Score;
	Graph Win;
	Graph Lose;
	Graph UI;

	vector <_Score> Record;
};

