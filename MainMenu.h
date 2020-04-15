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
	Virus virus = Virus(10,15,8,12,1.0,5,0.01);
	int state_count = 10;
	int population_count = 50000;
	int initial_infect = 10;
	int min_activity_level = 5;
	int max_activity_level = 12;
	double testing_kits_per_region = 1.0;
	double medical_cap_per_region = 2.0;
	double min_merchant_salary = 0.45; double max_merchant_salary = 0.55;
	double min_worker_salary = 0.15; double max_worker_salary = 0.25;
	vector<double> typeDistribution = {0.3,0.3,0.4};
	int initial_scan_limit = 0;
	Time time_limit = Time(6,23);
	int dead_limit = int(1e4);
	double min_seconds_between_hour = 1.0;
	public:
	void displayMainMenu(); //main menu
	void displaySettings(); //settings menu
	void displayHelpMenu(); //tutorial menu
	void displayExitMenu(); //exit menu
	void displayCredits(); //credits menu 
	void newGame(); //creates and plays new instance of game
	static void maximizeWindow(); //maximize window
};
#endif
