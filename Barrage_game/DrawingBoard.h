#pragma once
#include <Windows.h>
#include <string>
using namespace std;

enum DrawingBoard_STATUS
{
	DRAWING,
	END,
};

enum DrawRole
{
	HERO,
	MONSTER,
	BOSS,
};

class DrawingBoard
{
public:
	DrawingBoard();

	void draw(DrawRole role);

	~DrawingBoard();
private:
	HANDLE			hConsoleOutput;					//获取标准输出句柄
	HANDLE			hConsoleInput;					//获取标准输入句柄
	DWORD			dwNumberOfEventsRead;			//记录事件数量
	INPUT_RECORD	inputRecord;					//输入记录

	char			OutputCharacter[2];				//记录鼠标左右键的字符
	COORD			crHome;							//坐标{0，0}
	COORD			crPos;							//记录光标位置
	bool			bCaps;							//大写锁定键是否开启
	bool			bShift;							//Shift是否被按下
	bool			bCtrl_L, bCtrl_R;				//Ctrl是否被按下
	bool			bAlt_L, bAlt_R;					//Alt是否被按下

	DrawingBoard_STATUS status;
	DrawRole role;
	SMALL_RECT srWindowRect;

	int graph_l, graph_w;
	int the_num_of_ch;
	WORD infomationtext_color, printcharacter_color;
	WORD default_color;

	void initialDrawingBoard();
	void close();
	void tip();

	//输入检测
	void KeyTest();									
	//输出坐标和字符信息
	void PrintMessage();						

	//鼠标和键盘事件
	void switchMouseButtonStateEvent();
	void switchKeyButtonUpStateEvent();
	void switchKeyButtonDownStateEvent();
	void testControlKeyState();
	void testHotKey();

	//与输出字符有关的函数
	bool isPrintableCharacter(WORD &);
	void changeCharacter(WORD &);
	void upCharacter(WORD &);
	void downCharacter(WORD &);
	void gotoxy(COORD);
	void changeColor(WORD, char);

	//与清屏有关的函数
	COORD &setClearBeginPoint(COORD&, COORD&);
	void clearBoard_NextActionTest(COORD);
	void clearWholeBoard();

	//与文件操作有关的函数
	void getFileInfomation(string &);
	void saveFile(string &&, string &&);
	void output_RF_File(ofstream&, CONSOLE_SCREEN_BUFFER_INFO &, string);


	//与读取控制台字符有关的函数
	void readConcoleLineCharacter(COORD &, string &, int);
	void setDefault();
	void setInfomationAttribute(WORD);
	void setCharacterAttribute(WORD);
};

