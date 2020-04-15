#include "MainMenu.h"

void MainMenu::maximizeWindow()
{
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
}

void MainMenu::displayMainMenu() //main menu
{
	while(true)
	{
		system("cls");
		cout<<"Welcome to the Virus Infection Game!\n";
		int choice = IOHandler::askChoices(
					{"Start Game",
					  "Settings",
					  "Help",
					  "Credits",
					  "Exit"});
		switch(choice)
		{
			case 0:
			newGame();
			break;
			case 1:
			displaySettings();
			break;
			case 2:
			displayHelpMenu();
			break;
			case 3:
			displayCredits();
			break;
			case 4:
			displayExitMenu(); //exit from here
			break;
			default:
			assert(0);
		}
	}
}

void MainMenu::displaySettings() //settings menu
{
	system("cls");
	cout<<"Settings will be added later!\n";
	system("pause");
}

void MainMenu::displayHelpMenu() //tutorial menu
{
	system("cls");
	cout<<"Help will be added later!\n";
	system("pause");
}

void MainMenu::displayCredits() //credits menu 
{
	system("cls");
	cout<<"Credits will be added later!\n";
	system("pause");
}

void MainMenu::displayExitMenu() //exit menu
{
	system("cls");
	cout<<"Thanks for playing!\n";
	system("pause");
	exit(0); //terminates program
}

void MainMenu::newGame() //initializes new game
{
	system("cls");
	Game game;
	game.initGame(rows,cols,initial_testing_acc,initial_money,virus,
			   state_count,population_count,initial_infect,min_activity_level,max_activity_level,
			   testing_kits_per_region, medical_cap_per_region,
			   min_merchant_salary,max_merchant_salary,
			   min_worker_salary,max_worker_salary,
			   typeDistribution,initial_scan_limit,
			   time_limit,dead_limit,min_seconds_between_hour);
	while(game.checkWinCondition()==0)
	{
		game.startHour();
	}
	//game ends, return to main menu
}
