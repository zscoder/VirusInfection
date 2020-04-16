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
	while(true)
	{
		system("cls");
		cout<<"Current Settings:\n";
		cout<<"Difficulty: "<<difficulty<<'\n';
		cout<<"Delay between hours: "<<int(min_seconds_between_hour*1000)<<"ms\n";
		cout<<'\n';
		cout<<"Which setting do you want to change? (enter the number corresponding to the setting)\n";
		int choice = IOHandler::askChoices(
					{"Difficulty",
					  "Game Speed",
					  "None"
					});
		switch(choice)
		{
			case 0:
			changeDifficultyMenu();
			break;
			case 1:
			changeGameSpeedMenu();
			break;
			case 2:
			return ;
			break;
			default:
			assert(0);
		}
	}
}

void MainMenu::changeDifficultyMenu()
{
	system("cls");
	cout<<"Choose difficulty (enter the number corresponding to the setting):\n";
	int choice = IOHandler::askChoices(
				{"Easy",
				 "Normal",
				 "Hard",
				 "Insane",
				 "Hell"
				});
	switch(choice)
	{
		case 0: //easy
		initial_testing_acc = 0.4;
		virus = Virus(6,10,7,10,1.0,5,0.006); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
		initial_infect = 13;
		min_activity_level = 6;
		max_activity_level = 10;
		testing_kits_per_region = 3.0;
		medical_cap_per_region = 2.0;
		min_merchant_salary = 0.45;  max_merchant_salary = 0.55;
		min_worker_salary = 0.15; max_worker_salary = 0.25;
		typeDistribution = {0.3,0.3,0.4};
		initial_scan_limit = 0;
		difficulty = "Easy";
		time_limit = Time(13,23);
		dead_limit = 20000;
		cout<<"Difficulty set to easy.\n";
		break;
		case 1: //normal
		initial_testing_acc = 0.4;
		virus = Virus(7,12,7,10,1.3,5,0.0065); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
		initial_infect = 15;
		min_activity_level = 7;
		max_activity_level = 11;
		testing_kits_per_region = 2.5;
		medical_cap_per_region = 1.5;
		min_merchant_salary = 0.43;  max_merchant_salary = 0.53;
		min_worker_salary = 0.15; max_worker_salary = 0.25;
		typeDistribution = {0.3,0.3,0.4};
		initial_scan_limit = 0;
		difficulty = "Normal";
		time_limit = Time(12,23);
		dead_limit = 15000;
		cout<<"Difficulty set to normal.\n";
		break;
		case 2: //hard
		initial_testing_acc = 0.35;
		virus = Virus(11,14,6,9,1.4,5,0.0069); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
		initial_infect = 15;
		min_activity_level = 8;
		max_activity_level = 12;
		testing_kits_per_region = 2.5;
		medical_cap_per_region = 1.5;
		min_merchant_salary = 0.42;  max_merchant_salary = 0.52;
		min_worker_salary = 0.15; max_worker_salary = 0.25;
		typeDistribution = {0.3,0.3,0.4};
		initial_scan_limit = 0;
		difficulty = "Hard";
		time_limit = Time(11,23);
		dead_limit = 12500;
		cout<<"Difficulty set to hard.\n";
		break;
		case 3: //insane
		initial_testing_acc = 0.3;
		virus = Virus(13,16,5,9,1.8,6,0.0078); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
		initial_infect = 17;
		min_activity_level = 9;
		max_activity_level = 13;
		testing_kits_per_region = 2.0;
		medical_cap_per_region = 1.0;
		min_merchant_salary = 0.41;  max_merchant_salary = 0.51;
		min_worker_salary = 0.14; max_worker_salary = 0.24;
		typeDistribution = {0.3,0.3,0.4};
		initial_scan_limit = 0;
		difficulty = "Insane";
		time_limit = Time(10,23);
		dead_limit = 10000;
		cout<<"Difficulty set to insane.\n";
		break;
		case 4: //hell
		initial_testing_acc = 0.2;
		virus = Virus(14,18,5,8,2.1,7,0.0086); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
		initial_infect = 20;
		min_activity_level = 9;
		max_activity_level = 14;
		testing_kits_per_region = 2.0;
		medical_cap_per_region = 1.0;
		min_merchant_salary = 0.33;  max_merchant_salary = 0.45;
		min_worker_salary = 0.11; max_worker_salary = 0.21;
		typeDistribution = {0.4,0.2,0.4};
		initial_scan_limit = 0;
		difficulty = "Hell";
		time_limit = Time(9,23);
		dead_limit = 6666;
		cout<<"Difficulty set to hell.\n";
		break;
		default:
		assert(0);
	}
	system("pause");
}

void MainMenu::changeGameSpeedMenu()
{
	cout<<"Current delay between hours: "<<int(min_seconds_between_hour*1000)<<"ms\n";
	int x = 0;
	cout<<"Enter new delay (between 1000ms to 4000ms inclusive): ";
	x=IOHandler::getInt(1000,4000);
	min_seconds_between_hour = double(x)/double(1000);
	cout<<"New delay between hours: "<<x<<"ms\n";
	system("pause");
}

void MainMenu::displayHelpMenu() //tutorial menu
{
	system("cls");
	cout<<"A virus is spreading in your country! Your task is to ensure that the virus is properly contained in your country.\n";
	cout<<"You win if every infected person alive in your country is hospitalized.\n";
	cout<<"You lose when the time limit is reached or there are too many deaths in the country.\n";
	cout<<"Your country is divided into several states, each of which consists of several connected regions.\n";
	cout<<"Every hour, some citizens will move around the country which might spread the virus. Also, some of the citizens work which will generate funds for the country.\n";
	cout<<"Every 3 hours, each state will perform random tests (based on the number of testing kits) within the state to detect new infected citizens.\n";
	cout<<'\n';
	cout<<"You have several tools at your disposal to stop the virus. "; IOHandler::coutc("Press any key during the game to activate the control panel.\n", IOHandler::LIGHTCYAN);
	cout<<'\n';
	cout<<left<<setw(60)<<"Increase testing accuracy";
	cout<<"Increases the probability of finding an infected person via testing.\n";
	cout<<left<<setw(60)<<"Increase testing kits";
	cout<<"Increase the amount of tests performed in the state.\n";
	cout<<left<<setw(60)<<"Increase medical capacity";
	cout<<"Increase the amount of patients that can be hospitalized in the state.\n";
	cout<<left<<setw(60)<<"Ignore patients";
	cout<<"Abandon some of the sickest patients in the state hospital.\n";
	cout<<left<<setw(60)<<"Scan region";
	cout<<"Manually scan a region to detect infected citizens in a region.\n";
	cout<<left<<setw(60)<<"Increase scan limit";
	cout<<"Increase the amount of tests done during manual scans.\n";
	cout<<left<<setw(60)<<"Lockdown region/state";
	cout<<"Prevents people from inside the region/state to leave the region/state.\n";
	cout<<left<<setw(60)<<"";
	cout<<"Some work might be suspended.\n";
	cout<<left<<setw(60)<<"";
	cout<<"Activity of citizens within the state/region is decreased.\n";
	cout<<left<<setw(60)<<"Activate movement control in state";
	cout<<"Prevents people from inside the state from leaving their respective regions.\n";
	cout<<left<<setw(60)<<"";
	cout<<"All work in the state is suspended.\n";
	cout<<left<<setw(60)<<"";
	cout<<"Activity of citizens within the state/region is severely restricted.\n";
	cout<<right;
	system("pause");
}

void MainMenu::displayCredits() //credits menu 
{
	system("cls");
	cout<<"Credits will be added later.\n";
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
			   difficulty,
			   time_limit,dead_limit,min_seconds_between_hour);
	while(game.checkWinCondition()==0)
	{
		game.startHour();
	}
	//game ends, return to main menu
}
