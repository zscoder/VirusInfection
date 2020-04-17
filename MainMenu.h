#include "Game.h"

#ifndef MAIN_MENU_H
#define MAIN_MENU_H //header guards
class MainMenu
{
	private:
	//contains settings to be added later
	int rows = 8;
	int cols = 8;
	double initial_testing_acc = 0.4;
	long long initial_money = 0;
	Virus virus = Virus(6,10,7,10,1.0,5,0.006); //min_incubation, max_incubation, min_lethality, max_lethality, cure_difficulty, min_cure_hours, spread_difficulty
	int state_count = 8;
	int population_count = 50000;
	int initial_infect = 13;
	int min_activity_level = 6;
	int max_activity_level = 10;
	double testing_kits_per_region = 3.0;
	double medical_cap_per_region = 2.0;
	double min_merchant_salary = 0.5; double max_merchant_salary = 0.6;
	double min_worker_salary = 0.2; double max_worker_salary = 0.3;
	vector<double> typeDistribution = {0.4,0.4,0.2};
	int initial_scan_limit = 0;
	string difficulty = "Easy";
	Time time_limit = Time(13,23);
	int dead_limit = 20000;
	double min_seconds_between_hour = 2.0;
	public:
	void displayMainMenu(); //main menu
	void changeDifficultyMenu(); //difficulty menu
	void changeGameSpeedMenu(); //game speed menu
	void displaySettings(); //settings menu
	void displayHelpMenu(); //tutorial menu
	void displayExitMenu(); //exit menu
	void displayCredits(); //credits menu 
	void newGame(); //creates and plays new instance of game
	static void maximizeWindow(); //maximize window
};
#endif
