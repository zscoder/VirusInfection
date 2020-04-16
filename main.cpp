#include "Game.h"
#include "State.h"
#include "Region.h"
#include "Wanderer.h"
#include "Merchant.h"
#include "Worker.h"
#include "MainMenu.h"

using namespace std;

void main2()
{
	SetConsoleTitle("Virus Infection Game");
	MainMenu::maximizeWindow();
	MainMenu menu;
	menu.displayMainMenu();
}

int main()
{
	main2();
	system("pause");
}
