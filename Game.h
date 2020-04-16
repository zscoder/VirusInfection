#include "Grid.h"
#include "Random.h"
#include "Time.h"
#include "IOHandler.h"
#include "Virus.h"

#ifndef GAME_H
#define GAME_H //header guards

class Person;
class Region;
class State;
class Merchant;

class Game
{
	//Game constants
	public:
	static const int MAXN = 3055;
	static const int MAX_MAP_SIZE = 10;
	static const int AREA_PER_GRID = 250;
	static const int HOURS_BETWEEN_TESTS = 3;
	static const int WORK_RADIUS = AREA_PER_GRID/5;
	static const int MIN_START_HR = 6;
	static const int MAX_START_HR = 8;
	static const int MIN_END_HR = 17;
	static const int MAX_END_HR = 19;
	static const int MAX_SCANS = 5;
	static const int SCAN_RECHARGE_TIME = 6; //scan recharge rate
	static const int MAX_STATE_LOCKDOWN = 2;
	static const int STATE_LOCKDOWN_RECHARGE_TIME = 10; //state lockdown recharge rate
	static const int MAX_REGION_LOCKDOWN = 6;
	static const int REGION_LOCKDOWN_RECHARGE_TIME = 12; //region lockdown recharge rate
	static const int MAX_MOVEMENT_CONTROL = 1; //deactivate doesn't count
	static const int MOVEMENT_CONTROL_RECHARGE_TIME = 15;
	static const int PATIENT_IGNORE_LIMIT_RECHARGE_TIME = 3; 
	static const int MAX_SCAN_LIMIT = 500; //max scan limit = 500
	static const int MEDICAL_CAPACITY_RECHARGE_TIME = 4; //resets medical capacity upgrade limit every few hours
	static const int TESTING_KIT_RECHARGE_TIME = 4; //resets testing kits upgrade limit every few hours
	static const int TESTING_KIT_COST = 200; //cost to increase testing kits by 1
	static const int MEDICAL_CAPACITY_COST = 1250; //cost to increase medical cap by 1
	
	private:
	//Main variables
	Grid board = Grid(MAXN,MAXN);
	string difficulty="";
	int row_size=0; 
	int col_size=0; //we have row_size*col_size regions
	int scans_remaining=MAX_SCANS; //number of scans remaining
	int state_lockdown_remaining=1;
	int region_lockdown_remaining=3;
	int state_movement_control_remaining=0;
	int patient_ignore_remaining=0;
	vector<Person*> active_list; //list of active people
	vector<Person*> detected_list; //list of detected people (including hospitalized)
	vector<Person*> dead_list; //list of dead people
	vector<State*> state_list; //list of states
	static map<string,string> command_tooltips; //tooltip for commands
	Region* region_list[MAX_MAP_SIZE][MAX_MAP_SIZE]; //our main map
	double testing_accuracy; //probability that a person with virus will be tested positive
	long long money; //current economy status
	Virus virus;
	double min_seconds_between_hour = 1.0;
	Time game_time; //time in game
	Time time_limit = Time(int(1e8),int(1e8)); //time limit
	int dead_limit = int(1e9); //maximum number of deaths allowed
	int scan_limit = 0; //# of people scanned in each region
	int medical_capacity_limit = 0;
	int testing_kit_limit = 0;
	bool end_game = false; //did user end the game?
	public:	
	~Game(); //destroy all the objects in the lists, important to prevent memory leak
	bool expandState(int x, int y); //expand state at this region
	void generateStates(int state_count); //generate state_count states from the map
	void initGame(int _rows, int _cols, double initial_testing_acc, long long initial_money, Virus virus, 
				  int state_count, int population_count, int initial_infect, int min_activity_level, int max_activity_level, 
				  double testing_kits_per_region, double medical_cap_per_region,
				  double min_merchant_salary, double max_merchant_salary,
				  double min_worker_salary, double max_worker_salary,
				  vector<double> typeDistribution, int initial_scan_limit = 0, 
				  const string &difficulty = "",
				  Time time_limit = Time(int(1e8),int(1e8)), int dead_limit = int(1e9), double min_seconds_between_hour = 1.0); //initalizes a new game. call this whenever we create a new game
				  
	void processCommand(const string &s); //given a command by the user, perform its actions
	
	//Menus related to the user commands
	void upgradeMenu(); //call this when user goes to upgrade menu
	void helpMenu(); //call this when user goes to help menu
	void displayToolTip(const string &s);
	//Functions related to the user commands
	void upgradeAccuracy(const vector<string> &word_list);
	void upgradeScan(const vector<string> &word_list);
	void upgradeTest(const vector<string> &word_list);
	void upgradeMedical(const vector<string> &word_list);
	void ignore(const vector<string> &word_list);
	void scan(const vector<string> &word_list);
	void lockdown(const vector<string> &word_list);
	void unlockdown(const vector<string> &word_list);
	void movementControl(const vector<string> &word_list);
	void unmovementControl(const vector<string> &word_list);
	//Tooltips related to the user commands
	void upgradeAccuracyToolTip(const vector<string> &word_list);
	void upgradeScanToolTip(const vector<string> &word_list);
	void upgradeTestToolTip(const vector<string> &word_list);
	void upgradeMedicalToolTip(const vector<string> &word_list);
	void ignoreToolTip(const vector<string> &word_list);
	void scanToolTip(const vector<string> &word_list);
	void lockdownToolTip(const vector<string> &word_list);
	void unlockdownToolTip(const vector<string> &word_list);
	void movementControlToolTip(const vector<string> &word_list);
	void unmovementControlToolTip(const vector<string> &word_list);
	//Maximum limit for user commands
	int upgradeAccuracyMax(); 
	int upgradeScanMax();
	int upgradeTestMax(const State *s);
	int upgradeMedicalMax(const State *s);
	int ignoreMax(const State *s);
	static long long getAccuracyUpgradeCost(int level);
	static long long getScanLimitUpgradeCost(int level);
	static int getMaxPatientIgnore(const Time &t); //max # of patients that you can ignore
	static int getMaxMedicalCapacityUpgrade(const Time &t); 
	static int getMaxTestingKitUpgrade(const Time &t);
	
	void stepOn(int x, int y) {board.add(x,y,1);} //new guy steps on (x,y)
	void resetBoard() {board.resetGrid();} //set board elements to 0
	bool commandPanel(); //when user presses any key, switch to command mode until user exits
	void updateHour(); //updates performed at the end of hour, call at end of startHour()
	void displayMap(); //displays the map
	void displayStats(); //displays the current stats
	void startHour(); //starts a new hour (displays new map, stats and etc)
	int checkWinCondition(); //check if the current state ends the game (-1 = lose, 1 = win, 0 = not end)
	void endGame(int win_condition); //ends the game and display the user's results, -1 = lose, 1 = win
	
	Virus getVirus() const {return virus;}
	double getInfectionProbability(int visits); //get the infection probability based on # of visits
	Region* getRegion(int x, int y) const {return region_list[x][y];}
	State* getState(int x, int y) const; //get state at (x,y)
	int getBoardCount(int x, int y) const {return board.at(x,y);}
	int getRowSize() const {return row_size;}
	int getColSize() const {return col_size;}
	bool isValidRegion(int x, int y) const {return (x>=0&&x<row_size&&y>=0&&y<col_size);}
	double getMinSecondsBetweenHour() const {return min_seconds_between_hour;}
	double getTestingAccuracy() const {return testing_accuracy;}
	long long getMoney() const {return money;}
	void addMoney(long long x) {money += x;}
	bool isTestTime(); //is the current time a global testing time?
	int getScanLimit() const {return scan_limit;}
	Time getGameTime() const {return game_time;}
	int getWanderRange(Person *p); //get the range for wandering for the person 
	int getMerchantWorkRange(Merchant *m); //get the work range for a merchant (specific to merchant class)
};

#endif


