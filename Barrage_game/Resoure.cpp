#include "Resoure.h"

#include <fstream>
#include <iostream>
using namespace std;

Resoure::Resoure()
{
	rf = ".rf";
	cl = ".color";

	setHeroGraph();
	setBossGraph();
	setMonsterGraph();
	setMeauGraph();
	setScoreGraph();
	setWinGraph();
	setLoseGraph();
	setUIGraph();
	setSubmeauGraph();

	Record.clear();
	readRecord();
}

void Resoure::reset()
{
	setScoreGraph();
	setWinGraph();
	setLoseGraph();
}

void Resoure::resetHero()
{
	Hero.clear();
	setHeroGraph();
}

void Resoure::resetBoss()
{
	Boss.clear();
	setBossGraph();
}

void Resoure::resetMonster()
{
	Monster.clear();
	setMonsterGraph();
}

void Resoure::resetMeau()
{
	Meau.clear();
	setMeauGraph();
}

Graph Resoure::getHeroGraph(int num)
{
	if (num <= Hero.size() && num > 0)
		return Hero[num - 1];
	return Hero[0];
}

Graph Resoure::getBossGraph(int num)
{
	if (num <= Boss.size() && num > 0)
		return Boss[num - 1];
	return Boss[0];
}

Graph Resoure::getMonsterGraph(int num)
{
	if (num <= Monster.size() && num > 0)
		return Monster[num - 1];
	return Monster[0];
}

Graph Resoure::getMeauGraph(int num)
{
	if (num <= Meau.size() && num > 0)
		return Meau[num - 1];
	return Meau[0];
}

Graph Resoure::getSubmeauGraph()
{
	return Submeau;
}

Graph Resoure::getScoreGraph()
{
	return Score;
}

Graph Resoure::getWinGraph()
{
	return Win;
}

Graph Resoure::getLoseGraph()
{
	return Lose;
}

Graph Resoure::getUIGraph()
{
	return UI;
}

_Score Resoure::getScore(int num)
{
	return Record[num];
}

void Resoure::updateScore(int num, string & name, long score)
{
	_Score temp;
	temp.name = name;
	temp.score = score;
	Record.insert(Record.begin()+num, temp);
	Record.pop_back();

	string path = ".\\Resoure\\Record.data";
	ofstream os(path, ios::out | ios::binary);
	for (int i = 0; i < 15; i++)
	{
		os.write(reinterpret_cast<char*>( &Record[i].name ), 8 * sizeof(char));
		os.write(reinterpret_cast<char*>( &Record[i].score ), sizeof(long));
	}

	os.close();
}

int Resoure::getHeroGraphCount()
{
	return Hero.size();
}

int Resoure::getBossGraphCount()
{
	return Boss.size();
}

int Resoure::getMonsterGraphCount()
{
	return Monster.size();
}

int Resoure::getMeauGraphCount()
{
	return Meau.size();
}

Resoure::~Resoure()
{
	Hero.clear();
	Boss.clear();
	Monster.clear();
}

void Resoure::setHeroGraph()
{
	string path = ".\\Resoure\\Hero\\Hero_";
	string temp;
	for (int i = 1;; i++)
	{
		ss.clear();
		ss << i;
		ss >> temp;
		temp = path + temp;
		ifstream getgraph(temp + rf, ios::in);
		ifstream getcolor(temp + cl, ios::in | ios::binary);
		if (getgraph && getcolor)
		{
			Graph graph;
			graph.the_num_of_ch = 0;
			graph.data.clear();

			char ch_buf = 0;
			WORD color_buf;
			record data_buf;

			short x = 0, y = 0;
			for (; getgraph.peek() != EOF; y++)
			{
				x = 0;
				for (; ch_buf != '\n'; x++)
				{
					ch_buf = getgraph.get();

					if (ch_buf != '\n')
						getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
					if (ch_buf != ' ' && ch_buf != '\n')
					{
						graph.the_num_of_ch++;
						data_buf.ch = ch_buf;
						data_buf.color = color_buf;
						data_buf.chPos = { x,y };
						graph.data.push_back(data_buf);
						if (color_buf == ( FOREGROUND_RED | FOREGROUND_INTENSITY ))
							graph.centerPos = { x,y };
					}
				}
				ch_buf = 0;
			}
			graph.size = { x - 1,y };
			Hero.push_back(graph);
		}
		else
			break;
	}
}

void Resoure::setBossGraph()
{
	string path = ".\\Resoure\\Boss\\Boss_";
	string temp;
	for (int i = 1;; i++)
	{

		ss.clear();
		ss << i;
		ss >> temp;
		temp = path + temp;
		ifstream getgraph(temp + rf, ios::in);
		ifstream getcolor(temp + cl, ios::in | ios::binary);
		if (getgraph && getcolor)
		{
			Graph graph;
			graph.the_num_of_ch = 0;
			graph.data.clear();

			char ch_buf = 0;
			WORD color_buf;
			record data_buf;

			short x = 0, y = 0;
			for (; getgraph.peek() != EOF; y++)
			{
				x = 0;
				for (; ch_buf != '\n'; x++)
				{
					ch_buf = getgraph.get();

					if (ch_buf != '\n')
						getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
					if (ch_buf != ' ' && ch_buf != '\n')
					{
						graph.the_num_of_ch++;
						data_buf.ch = ch_buf;
						data_buf.color = color_buf;
						data_buf.chPos = { x,y };
						graph.data.push_back(data_buf);
					}
				}
				ch_buf = 0;
			}

			graph.size = { x - 1,y };
			graph.centerPos.X = ( graph.size.X - 1 ) / 2;
			graph.centerPos.Y = ( graph.size.Y - 1 ) / 2;
			Boss.push_back(graph);
		}
		else
			break;
	}
}

void Resoure::setMonsterGraph()
{
	string path = ".\\Resoure\\Monster\\Monster_";
	string temp;
	for (int i = 1;; i++)
	{
		ss.clear();
		ss << i;
		ss >> temp;
		temp = path + temp;
		ifstream getgraph(temp + rf, ios::in);
		ifstream getcolor(temp + cl, ios::in | ios::binary);
		if (getgraph && getcolor)
		{
			Graph graph;
			graph.the_num_of_ch = 0;
			graph.data.clear();

			char ch_buf = 0;
			WORD color_buf;
			record data_buf;

			short x = 0, y = 0;
			for (; getgraph.peek() != EOF; y++)
			{
				x = 0;
				for (; ch_buf != '\n'; x++)
				{
					ch_buf = getgraph.get();

					if (ch_buf != '\n')
						getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
					if (ch_buf != ' ' && ch_buf != '\n')
					{
						graph.the_num_of_ch++;
						data_buf.ch = ch_buf;
						data_buf.color = color_buf;
						data_buf.chPos = { x,y };
						graph.data.push_back(data_buf);
					}
				}
				ch_buf = 0;
			}

			graph.size = { x - 1,y };
			graph.centerPos.X = ( graph.size.X - 1 ) / 2;
			graph.centerPos.Y = ( graph.size.Y - 1 ) / 2;
			Monster.push_back(graph);
		}
		else
			break;
	}
}

void Resoure::setMeauGraph()
{
	string path = ".\\Resoure\\UI\\MEAU_";
	string temp;
	for (int i = 1;; i++)
	{

		ss.clear();
		ss << i;
		ss >> temp;
		temp = path + temp;
		ifstream getgraph(temp + rf, ios::in);
		ifstream getcolor(temp + cl, ios::in | ios::binary);
		if (getgraph && getcolor)
		{
			Graph graph;
			graph.the_num_of_ch = 0;
			graph.data.clear();

			char ch_buf = 0;
			WORD color_buf;
			record data_buf;

			short x = 0, y = 0;
			for (; getgraph.peek() != EOF; y++)
			{
				x = 0;
				for (; ch_buf != '\n'; x++)
				{
					ch_buf = getgraph.get();

					if (ch_buf != '\n')
						getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
					if (ch_buf != ' ' && ch_buf != '\n')
					{
						graph.the_num_of_ch++;
						data_buf.ch = ch_buf;
						data_buf.color = color_buf;
						data_buf.chPos = { x,y };
						graph.data.push_back(data_buf);
					}
				}
				ch_buf = 0;
			}

			graph.size = { x - 1,y };
			graph.centerPos.X = ( graph.size.X - 1 ) / 2;
			graph.centerPos.Y = ( graph.size.Y - 1 ) / 2;
			Meau.push_back(graph);
		}
		else
			break;
	}
}

void Resoure::setSubmeauGraph()
{
	string path = ".\\Resoure\\UI\\SUBMEAU";

	ifstream getgraph(path + rf, ios::in);
	ifstream getcolor(path + cl, ios::in | ios::binary);
	if (getgraph && getcolor)
	{
		Submeau.the_num_of_ch = 0;
		Submeau.data.clear();

		char ch_buf = 0;
		WORD color_buf;
		record data_buf;

		short x = 0, y = 0;
		for (; getgraph.peek() != EOF; y++)
		{
			x = 0;
			for (; ch_buf != '\n'; x++)
			{
				ch_buf = getgraph.get();

				if (ch_buf != '\n')
					getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
				if (ch_buf != ' ' && ch_buf != '\n')
				{
					Submeau.the_num_of_ch++;
					data_buf.ch = ch_buf;
					data_buf.color = color_buf;
					data_buf.chPos = { x,y };
					Submeau.data.push_back(data_buf);
				}
			}
			ch_buf = 0;
		}

		Submeau.size = { x - 1,y };
		Submeau.centerPos.X = ( Submeau.size.X - 1 ) / 2;
		Submeau.centerPos.Y = ( Submeau.size.Y - 1 ) / 2;
	}
}

void Resoure::setScoreGraph()
{
	string path = ".\\Resoure\\UI\\SCORE";

	ifstream getgraph(path + rf, ios::in);
	ifstream getcolor(path + cl, ios::in | ios::binary);
	if (getgraph && getcolor)
	{
		Score.the_num_of_ch = 0;
		Score.data.clear();

		char ch_buf = 0;
		WORD color_buf;
		record data_buf;

		short x = 0, y = 0;
		for (; getgraph.peek() != EOF; y++)
		{
			x = 0;
			for (; ch_buf != '\n'; x++)
			{
				ch_buf = getgraph.get();

				if (ch_buf != '\n')
					getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
				if (ch_buf != ' ' && ch_buf != '\n')
				{
					Score.the_num_of_ch++;
					data_buf.ch = ch_buf;
					data_buf.color = color_buf;
					data_buf.chPos = { x,y };
					Score.data.push_back(data_buf);
				}
			}
			ch_buf = 0;
		}

		Score.size = { x - 1,y };
		Score.centerPos.X = ( Score.size.X - 1 ) / 2;
		Score.centerPos.Y = ( Score.size.Y - 1 ) / 2;
	}

}

void Resoure::setWinGraph()
{
	string path = ".\\Resoure\\UI\\WIN";

	ifstream getgraph(path + rf, ios::in);
	ifstream getcolor(path + cl, ios::in | ios::binary);
	if (getgraph && getcolor)
	{
		Win.the_num_of_ch = 0;
		Win.data.clear();

		char ch_buf = 0;
		WORD color_buf;
		record data_buf;

		short x = 0, y = 0;
		for (; getgraph.peek() != EOF; y++)
		{
			x = 0;
			for (; ch_buf != '\n'; x++)
			{
				ch_buf = getgraph.get();

				if (ch_buf != '\n')
					getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
				if (ch_buf != ' ' && ch_buf != '\n')
				{
					Win.the_num_of_ch++;
					data_buf.ch = ch_buf;
					data_buf.color = color_buf;
					data_buf.chPos = { x,y };
					Win.data.push_back(data_buf);
				}
			}
			ch_buf = 0;
		}

		Win.size = { x - 1,y };
		Win.centerPos.X = ( Win.size.X - 1 ) / 2;
		Win.centerPos.Y = ( Win.size.Y - 1 ) / 2;
	}
}

void Resoure::setLoseGraph()
{
	string path = ".\\Resoure\\UI\\LOSE";

	ifstream getgraph(path + rf, ios::in);
	ifstream getcolor(path + cl, ios::in | ios::binary);
	if (getgraph && getcolor)
	{
		Lose.the_num_of_ch = 0;
		Lose.data.clear();

		char ch_buf = 0;
		WORD color_buf;
		record data_buf;

		short x = 0, y = 0;
		for (; getgraph.peek() != EOF; y++)
		{
			x = 0;
			for (; ch_buf != '\n'; x++)
			{
				ch_buf = getgraph.get();

				if (ch_buf != '\n')
					getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
				if (ch_buf != ' ' && ch_buf != '\n')
				{
					Lose.the_num_of_ch++;
					data_buf.ch = ch_buf;
					data_buf.color = color_buf;
					data_buf.chPos = { x,y };
					Lose.data.push_back(data_buf);
				}
			}
			ch_buf = 0;
		}

		Lose.size = { x - 1,y };
		Lose.centerPos.X = ( Lose.size.X - 1 ) / 2;
		Lose.centerPos.Y = ( Lose.size.Y - 1 ) / 2;
	}
}

void Resoure::setUIGraph()
{
	string path = ".\\Resoure\\UI\\UI";

	ifstream getgraph(path + rf, ios::in);
	ifstream getcolor(path + cl, ios::in | ios::binary);
	if (getgraph && getcolor)
	{
		UI.the_num_of_ch = 0;
		UI.data.clear();

		char ch_buf = 0;
		WORD color_buf;
		record data_buf;

		short x = 0, y = 0;
		for (; getgraph.peek() != EOF; y++)
		{
			x = 0;
			for (; ch_buf != '\n'; x++)
			{
				ch_buf = getgraph.get();

				if (ch_buf != '\n')
					getcolor.read(reinterpret_cast<char*>( &color_buf ), sizeof(WORD));
				if (ch_buf != ' ' && ch_buf != '\n')
				{
					UI.the_num_of_ch++;
					data_buf.ch = ch_buf;
					data_buf.color = color_buf;
					data_buf.chPos = { x,y };
					UI.data.push_back(data_buf);
				}
			}
			ch_buf = 0;
		}

		UI.size = { x - 1,y };
		UI.centerPos.X = ( UI.size.X - 1 ) / 2;
		UI.centerPos.Y = ( UI.size.Y - 1 ) / 2;
	}
}

void Resoure::readRecord()
{
	string path = ".\\Resoure\\Record.data";
	ifstream is(path, ios::in | ios::binary);

	_Score temp = { "XXXXXXXX",0 };

	if (is)
	{
		while (is.peek() != EOF)
		{
			is.read(reinterpret_cast<char*>( &temp.name ), 8 * sizeof(char));
			is.read(reinterpret_cast<char*>( &temp.score ), sizeof(long));
			Record.push_back(temp);
		}
	}
	else
	{
		ofstream os(path, ios::out | ios::binary);
		for (int i = 0; i < 15; i++)
		{
			os.write(reinterpret_cast<char*>( &temp.name ), 8 * sizeof(char));
			os.write(reinterpret_cast<char*>( &temp.score ), sizeof(long));
		}

		os.close();

		while (Record.size() < 15)
		{
			Record.push_back(temp);
		}
	}
}
