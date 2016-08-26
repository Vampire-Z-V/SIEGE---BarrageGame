#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "Meau.h"
#include "Resoure.h"
#include "Console.h"

Console console;
Resoure resoure;
LARGE_INTEGER freq;
int choseHero = 1;

int main()
{
	QueryPerformanceFrequency(&freq); 
	Meau start;
	start.displayMeau();

	return 0;
}