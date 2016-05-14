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
	HANDLE			hConsoleOutput;					//��ȡ��׼������
	HANDLE			hConsoleInput;					//��ȡ��׼������
	DWORD			dwNumberOfEventsRead;			//��¼�¼�����
	INPUT_RECORD	inputRecord;					//�����¼

	char			OutputCharacter[2];				//��¼������Ҽ����ַ�
	COORD			crHome;							//����{0��0}
	COORD			crPos;							//��¼���λ��
	bool			bCaps;							//��д�������Ƿ���
	bool			bShift;							//Shift�Ƿ񱻰���
	bool			bCtrl_L, bCtrl_R;				//Ctrl�Ƿ񱻰���
	bool			bAlt_L, bAlt_R;					//Alt�Ƿ񱻰���

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

	//������
	void KeyTest();									
	//���������ַ���Ϣ
	void PrintMessage();						

	//���ͼ����¼�
	void switchMouseButtonStateEvent();
	void switchKeyButtonUpStateEvent();
	void switchKeyButtonDownStateEvent();
	void testControlKeyState();
	void testHotKey();

	//������ַ��йصĺ���
	bool isPrintableCharacter(WORD &);
	void changeCharacter(WORD &);
	void upCharacter(WORD &);
	void downCharacter(WORD &);
	void gotoxy(COORD);
	void changeColor(WORD, char);

	//�������йصĺ���
	COORD &setClearBeginPoint(COORD&, COORD&);
	void clearBoard_NextActionTest(COORD);
	void clearWholeBoard();

	//���ļ������йصĺ���
	void getFileInfomation(string &);
	void saveFile(string &&, string &&);
	void output_RF_File(ofstream&, CONSOLE_SCREEN_BUFFER_INFO &, string);


	//���ȡ����̨�ַ��йصĺ���
	void readConcoleLineCharacter(COORD &, string &, int);
	void setDefault();
	void setInfomationAttribute(WORD);
	void setCharacterAttribute(WORD);
};

