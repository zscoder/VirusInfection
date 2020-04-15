#include "Game.h"
#include "Wanderer.h"
#include "Merchant.h"
#include "Worker.h"
#include "State.h"

Game::~Game() //destroy dynamically allocated elements
{
	for(Person* p:active_list) delete p;
	for(Person* p:detected_list) delete p;
	for(Person* p:dead_list) delete p;
	for(State* s:state_list) delete s;
	for(int i=0;i<row_size;i++)
	{
		for(int j=0;j<col_size;j++)
		{
			delete region_list[i][j];
		}
	}
}

State* Game::getState(int x, int y) const
{
	return region_list[x][y]->getState();
}

bool Game::expandState(int x, int y) //expand region
{
	vector<pair<int,int> > directions = {{-1,0},{1,0},{0,-1},{0,1}};
	srand(time(NULL)); random_shuffle(directions.begin(),directions.end());
	for(int i=0;i<int(directions.size());i++)
	{
		int x2 = x+directions[i].first; int y2 = y+directions[i].second;
		if(isValidRegion(x2,y2)&&(region_list[x2][y2]==NULL))
		{
			region_list[x2][y2] = new Region(x2,y2,(x2*col_size+y2),region_list[x][y]->getState());
			region_list[x][y]->getState()->addRegion(region_list[x2][y2]);
			return true;
		}
	}
	return false; //failed to expand state
}

void Game::generateStates(int state_count) //generate states
{
	//we want to generate state_count connected states on the map
	//here, we start from state_count random initial points
	//while not regions are taken, pick a random taken region, and expand in a random direction. repeat until all regions are taken
	vector<pair<int,int> > possible_regions;
	for(int i=0;i<row_size;i++)
	{
		for(int j=0;j<col_size;j++)
		{
			possible_regions.push_back({i,j});
		}
	}
	srand(time(NULL)); random_shuffle(possible_regions.begin(),possible_regions.end());
	assert(state_count<=int(possible_regions.size())&&state_count>0); //we need enough states
	for(int i=0;i<state_count;i++)
	{
		int x = possible_regions[i].first; int y = possible_regions[i].second;
		region_list[x][y] = new Region(x,y,(x*col_size+y),state_list[i]);
		state_list[i]->addRegion(region_list[x][y]);
	}
	while(true) //while not all regions are owned by a state
	{
		vector<pair<int,int> > active_regions;
		for(int i=0;i<row_size;i++)
		{
			for(int j=0;j<col_size;j++)
			{
				if(region_list[i][j]==NULL) continue;
				active_regions.push_back({i,j});
			}
		}
		random_shuffle(active_regions.begin(),active_regions.end());
		bool expanded=0;
		for(int i=0;i<int(active_regions.size());i++)
		{
			int x=active_regions[i].first; int y=active_regions[i].second;
			expanded = expandState(x,y);
			if(expanded) break;
		}
		if(!expanded) break; //cannot expand anymore => all regions are taken
	}
}

void Game::initGame(int _rows, int _cols, double initial_testing_acc, long long initial_money, Virus virus, 
					int state_count, int population_count, int initial_infect, int min_activity_level, int max_activity_level, 
					double testing_kits_per_region, double medical_cap_per_region,
					double min_merchant_salary, double max_merchant_salary,
					double min_worker_salary, double max_worker_salary,
					vector<double> typeDistribution, int initial_scan_limit, 
					Time time_limit, int dead_limit, double min_seconds_between_hour) //initalizes a new game. call this whenever we create a new game
{
	//initialize variables
	row_size = _rows; col_size = _cols;
	resetBoard();
	cout<<"Initializing game world...\n";
	for(int i=0;i<row_size;i++)
	{
		for(int j=0;j<col_size;j++)
		{
			region_list[i][j] = NULL;
		}
	}
	testing_accuracy = initial_testing_acc;
	money = initial_money;
	Game::virus = virus;
	scan_limit = initial_scan_limit;
	Game::time_limit = time_limit;
	Game::dead_limit = dead_limit;
	Game::min_seconds_between_hour = min_seconds_between_hour;
	//initialize states
	cout<<"Initializing states...\n";
	char curchar = 'A';
	for(int i=0;i<state_count;i++)
	{
		State* s = new State(0,0,curchar); //int testing_kits, int medical_capacity, char id
		state_list.push_back(s);
		curchar++;
	}
	//assign region to states
	cout<<"Initializing regions...\n";
	generateStates(state_count);
	//update medical facilities for states
	for(int i=0;i<state_count;i++)
	{
		int state_size = state_list[i]->getStateSize();
		state_list[i]->addMedicalCap(floor(medical_cap_per_region*1.0*state_size));
		state_list[i]->addTestingKits(floor(testing_kits_per_region*1.0*state_size));
	}
	//assign people :( the hard part
	assert(typeDistribution.size()==3);
	//0 = merchant, 1 = worker, 2 = wanderer
	//probability that a given person will be of each type
	cout<<"Initializing citizens...\n";
	double sum = typeDistribution[0]+typeDistribution[1]+typeDistribution[2];
	assert(fabs(sum-1.0)<=double(1e-7)); //sum must be close to 1
	for(int i=0;i<population_count;i++)
	{
		int posx = Random::getRand(0,AREA_PER_GRID*row_size-1);
		int posy = Random::getRand(0,AREA_PER_GRID*col_size-1);
		int activity = Random::getRand(min_activity_level, max_activity_level);
		int id = i;
		Person *p;
		double prob = Random::getRand(0.0,1.0);
		if(prob<=typeDistribution[0])
		{
			//merchant
			p = new Merchant(posx,posy,activity,id,Random::getRand(min_merchant_salary,max_merchant_salary));
		}
		else if(prob<=typeDistribution[0]+typeDistribution[1])
		{
			//worker
			int workx,worky;
			do
			{
				workx = Random::getRand(max(0,posx-WORK_RADIUS),posx+WORK_RADIUS);
			}while((workx/AREA_PER_GRID != posx/AREA_PER_GRID)); //while work place and home place are in different regions
			do
			{
				worky = Random::getRand(max(0,posy-WORK_RADIUS),posy+WORK_RADIUS);
			}while((worky/AREA_PER_GRID != posy/AREA_PER_GRID)); //while work place and home place are in different regions
			int start_hr = Random::getRand(MIN_START_HR,MAX_START_HR);
			int end_hr = Random::getRand(MIN_END_HR,MAX_END_HR);
			p = new Worker(posx,posy,activity,id,Random::getRand(min_merchant_salary,max_merchant_salary),posx,posy,workx,worky,start_hr,end_hr);
		}
		else
		{
			//wanderer
			p = new Wanderer(posx,posy,activity,id);
		}
		active_list.push_back(p);
		Region* r = p->getRegion(*this);
		State* s = p->getState(*this);
		s->addActivePerson(p);
		r->addActivePerson(p);
	}
	vector<int> vec(population_count);
	for(int i=0;i<population_count;i++) vec[i]=i;
	random_shuffle(vec.begin(),vec.end());
	for(int i=0;i<initial_infect;i++) active_list[vec[i]]->infect(virus);
	//people distributed
	game_time = Time(0,0); //reset game time
	cout<<"Initialization complete!\n";
	system("pause");
}

map<string,string> Game::command_tooltips = {
{"h", "<h/help>: Displays help menu."},
{"help", "<h/help>: Displays help menu."},
{"e", "<e/exit>: Exits command panel."},
{"exit", "<e/exit>: Exits command panel."},
{"ua", "<ua/upgradeaccuracy> <integer>/<a>: Upgrades accuracy by <integer> levels (<a> = all). e.g. ua 3 or ua a"},
{"upgradeaccuracy", "<ua/upgradeaccuracy> <integer>/<a>: Upgrades accuracy by <integer> levels (<a> = all). e.g. ua 3 or ua a"},
{"us", "<us/upgradescan> <integer>/<a>: Increase scan limit by <integer> (<a> = all). e.g. us 30 or us a"},
{"upgradescan", "<us/upgradescan> <integer>/<a>: Increase scan limit by <integer> (<a> = all). e.g. us 30 or us a"},
{"ut", "<ut/upgradetest> <stateID> <integer>/<a>: Increase testing kits of <stateID> by <integer> (<a> = all). e.g. ut D 30 or ut F a"},
{"upgradetest", "<ut/upgradetest> <stateID> <integer>/<a>: Increase testing kits of <stateID> by <integer> (<a> = all). e.g. ut D 30 or ut F a"},
{"um", "<um/upgrademedical> <stateID> <integer>/<a>: Increase medical capacity of <stateID> by <integer> (<a> = all). e.g. um D 30 or um F a"},
{"upgrademedical", "<um/upgrademedical> <stateID> <integer>/<a>: Increase medical capacity of <stateID> by <integer> (<a> = all). e.g. um D 30 or um F a"},
{"i", "<i/ignore> <stateID> <integer>/<a>: Ignore the <integer> sickest hospitalized patients from <stateID> (<a> = all). e.g. i D 30 or i F a"},
{"ignore", "<i/ignore> <stateID> <integer>/<a>: Ignore the <integer> sickest hospitalized patients from <stateID> (<a> = all). e.g. i D 30 or i F a"},
{"s", "<s/scan> <regionID>: Scans the region <regionID> for virus carriers. e.g. s 39 or s 8"},
{"scan", "<s/scan> <regionID>: Scans the region <regionID> for virus carriers. e.g. s 39 or s 8"},
{"l", "<l/lockdown> <regionID>/<stateID>: Lockdown the region <regionID> or state <stateID>. e.g. l 39 or l F"},
{"lockdown", "<l/lockdown> <regionID>/<stateID>: Lockdown the region <regionID> or state <stateID>. e.g. l 39 or l F"},
{"ul", "<ul/unlockdown> <regionID>/<stateID>: Disables lockdown for the region <regionID> or state <stateID>. e.g. ul 49 or ul A"},
{"unlockdown", "<ul/unlockdown> <regionID>/<stateID>: Disables lockdown the region <regionID> or state <stateID>. e.g. ul 49 or ul A"},
{"mc", "<mc/movementcontrol> <stateID>: Applies movement control on state <stateID>. e.g. mc D or mc E"},
{"movementcontrol", "<mc/movementcontrol> <stateID>: Applies movement control on state <stateID>. e.g. mc D or mc E"},
{"umc", "<umc/unmovementcontrol> <stateID>: Disables movement control on state <stateID>. e.g. umc D or umc E"},
{"unmovementcontrol", "<umc/unmovementcontrol> <stateID>: Disables movement control on state <stateID>. e.g. umc D or umc E"},
{"q", "<q/query> <command>: Displays additional info on <command>. May contain more than one parameters. e.g. q s or q ut D"},
{"query", "<q/query> <command>: Displays additional info on <command>. May contain more than one parameters. e.g. q s or q ut D"}
};

				
void Game::processCommand(const string &s) //given a command by the user, perform its actions
{
	vector<string> word_list = IOHandler::toWordList(s);
	if(word_list.empty()) //empty list of commands
	{
		IOHandler::coutc("Error: No commands given.\n", IOHandler::LIGHTRED);
		return ;
	}
	if(word_list[0]=="h"||word_list[0]=="help")
	{
		helpMenu();
		return ;
	}
	if(int(word_list.size())==1&&(word_list[0]=="u"||word_list[0]=="upgrade"))
	{
		upgradeMenu();
		return ;
	}
	if(word_list.size()==1)
	{
		displayToolTip(word_list[0]); //show how to use command with parameters
		return ;
	}
	//branch based on command
	if(word_list[0]=="q"||word_list[0]=="query")
	{
		word_list.erase(word_list.begin()); //ignore the word q
		if(word_list[0]=="ua"||word_list[0]=="upgradeaccuracy")
		{
			upgradeAccuracyToolTip(word_list);
			return ;
		}
		if(word_list[0]=="us"||word_list[0]=="upgradescan")
		{
			upgradeScanToolTip(word_list);
			return ;
		}
		if(word_list[0]=="ut"||word_list[0]=="upgradetest")
		{
			upgradeTestToolTip(word_list);
			return ;
		}
		if(word_list[0]=="um"||word_list[0]=="upgrademedical")
		{
			upgradeMedicalToolTip(word_list);
			return ;
		}
		if(word_list[0]=="i"||word_list[0]=="ignore")
		{
			ignoreToolTip(word_list);
			return ;
		}
		if(word_list[0]=="s"||word_list[0]=="scan")
		{
			scanToolTip(word_list);
			return ;
		}
		if(word_list[0]=="l"||word_list[0]=="lockdown")
		{
			lockdownToolTip(word_list);
			return ;
		}
		if(word_list[0]=="ul"||word_list[0]=="unlockdown")
		{
			unlockdownToolTip(word_list);
			return ;
		}
		if(word_list[0]=="mc"||word_list[0]=="movementcontrol")
		{
			movementControlToolTip(word_list);
			return ;
		}
		if(word_list[0]=="umc"||word_list[0]=="unmovementcontrol")
		{
			unmovementControlToolTip(word_list);
			return ;
		}
		return ;
	}
	if(word_list[0]=="ua"||word_list[0]=="upgradeaccuracy")
	{
		upgradeAccuracy(word_list);
		return ;
	}
	if(word_list[0]=="us"||word_list[0]=="upgradescan")
	{
		upgradeScan(word_list);
		return ;
	}
	if(word_list[0]=="ut"||word_list[0]=="upgradetest")
	{
		upgradeTest(word_list);
		return ;
	}
	if(word_list[0]=="um"||word_list[0]=="upgrademedical")
	{
		upgradeMedical(word_list);
		return ;
	}
	if(word_list[0]=="i"||word_list[0]=="ignore")
	{
		ignore(word_list);
		return ;
	}
	if(word_list[0]=="s"||word_list[0]=="scan")
	{
		scan(word_list);
		return ;
	}
	if(word_list[0]=="l"||word_list[0]=="lockdown")
	{
		lockdown(word_list);
		return ;
	}
	if(word_list[0]=="ul"||word_list[0]=="unlockdown")
	{
		unlockdown(word_list);
		return ;
	}
	if(word_list[0]=="mc"||word_list[0]=="movementcontrol")
	{
		movementControl(word_list);
		return ;
	}
	if(word_list[0]=="umc"||word_list[0]=="unmovementcontrol")
	{
		unmovementControl(word_list);
		return ;
	}
	displayToolTip(word_list[0]); //invalid command
}

void Game::upgradeMenu()
{
	cout<<"Upgrade Commands:\n";
	cout<<setw(5)<<"Upgrade Test Accuracy (ua)"<<"        "<<command_tooltips["ua"]<<'\n';
	cout<<setw(5)<<"Upgrade Scan Limit (us)"<<"        "<<command_tooltips["us"]<<'\n';
	cout<<setw(5)<<"Upgrade Testing Kits (ut)"<<"        "<<command_tooltips["ut"]<<'\n';
	cout<<setw(5)<<"Upgrade Medical Capacity (um)"<<"        "<<command_tooltips["um"]<<'\n';
}

void Game::helpMenu()
{
	cout<<"Command list:\n";
	for(auto X:command_tooltips)
	{
		if(X.first.length()>=4) continue;
		cout<<setw(5)<<X.first<<"        "<<X.second<<'\n';
	}
}

void Game::upgradeAccuracy(const vector<string> &word_list)
{
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	int maxNumber = upgradeAccuracyMax();
	int number = maxNumber;
	if(word_list[1]!="a"&&word_list[1]!="all") number=IOHandler::toInt(word_list[1]);
	if(number<0) 
	{
		displayToolTip(word_list[0]);
		return ;
	}
	//under construction
	//each level adds 1 percent let's say
	if(number>maxNumber)
	{
		IOHandler::errorRange(0,maxNumber);
		return ;
	}
	int current_percentage = round(testing_accuracy*100.0);
	money-=getAccuracyUpgradeCost(number+current_percentage)-getAccuracyUpgradeCost(current_percentage);
	assert(money>=0);
	testing_accuracy+=0.01*double(number);
	cout<<"Testing accuracy is increased by "<<number<<"%.\n";
	cout<<"Budget remaining: $"<<money<<'\n';
}
	
void Game::upgradeScan(const vector<string> &word_list)
{
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	int maxNumber = upgradeScanMax();
	int number = maxNumber;
	if(word_list[1]!="a"&&word_list[1]!="all") number=IOHandler::toInt(word_list[1]);
	if(number<0) 
	{
		displayToolTip(word_list[0]);
		return ;
	}
	//under construction
	if(number>maxNumber)
	{
		IOHandler::errorRange(0,maxNumber);
		return ;
	}
	money-=getScanLimitUpgradeCost(number+scan_limit)-getScanLimitUpgradeCost(scan_limit);
	scan_limit+=number;
	cout<<"Scan limit is increased by "<<number<<".\n";
	cout<<"Budget remaining: $"<<money<<'\n';
}
	
void Game::upgradeTest(const vector<string> &word_list)
{
	if(word_list.size()!=3)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	if(word_list[1].length()!=1)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	char c = word_list[1][0];
	State *current_state = NULL;
	for(State *t:state_list)
	{
		if(tolower(t->getId())==c)
		{
			current_state=t; break;
		}
	}
	if(current_state==NULL)
	{
		IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	int maxNumber = upgradeTestMax(current_state);
	int number = maxNumber;
	if(word_list[2]!="a"&&word_list[2]!="all") number=IOHandler::toInt(word_list[2]);
	if(number<0) 
	{
		displayToolTip(word_list[0]);
		return ;
	}
	//under construction
	if(number>maxNumber)
	{
		IOHandler::errorRange(0,maxNumber);
		return ;
	}
	current_state->addTestingKits(number);
	testing_kit_limit-=number;
	money-=TESTING_KIT_COST*1LL*number;
	cout<<"Number of testing kits of state "<<char(toupper(c))<<" is increased by "<<number<<".\n";
	cout<<"Budget remaining: $"<<money<<'\n';
}

void Game::upgradeMedical(const vector<string> &word_list)
{
	if(word_list.size()!=3)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	if(word_list[1].length()!=1)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	char c = word_list[1][0];
	State *current_state = NULL;
	for(State *t:state_list)
	{
		if(tolower(t->getId())==c)
		{
			current_state=t; break;
		}
	}
	if(current_state==NULL)
	{
		IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	int maxNumber = upgradeMedicalMax(current_state);
	int number = maxNumber;
	if(word_list[2]!="a"&&word_list[2]!="all") number=IOHandler::toInt(word_list[2]);
	if(number<0) 
	{
		displayToolTip(word_list[0]);
		return ;
	}
	//under construction
	if(number>maxNumber)
	{
		IOHandler::errorRange(0,maxNumber);
		return ;
	}
	current_state->addMedicalCap(number);
	medical_capacity_limit-=number;
	money-=MEDICAL_CAPACITY_COST*1LL*number;
	cout<<"Medical capacity of state "<<char(toupper(c))<<" is increased by "<<number<<".\n";
	cout<<"Budget remaining: $"<<money<<'\n';
}

void Game::ignore(const vector<string> &word_list)
{
	if(word_list.size()!=3)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	if(word_list[1].length()!=1)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	char c = word_list[1][0];
	State *current_state = NULL;
	for(State *t:state_list)
	{
		if(tolower(t->getId())==c)
		{
			current_state=t; break;
		}
	}
	if(current_state==NULL)
	{
		IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	int maxNumber = ignoreMax(current_state);
	int number = maxNumber;
	if(word_list[2]!="a"&&word_list[2]!="all") number=IOHandler::toInt(word_list[2]);
	if(number<0) 
	{
		displayToolTip(word_list[0]);
		return ;
	}
	//under construction
	if(number>maxNumber)
	{
		IOHandler::errorRange(0,maxNumber);
		return ;
	}
	current_state->ignoreSickest(number);
	cout<<number<<" sickest patient(s) in state "<<char(toupper(c))<<" is/are ignored.\n";
	patient_ignore_remaining-=number;
}

void Game::scan(const vector<string> &word_list)
{
	if(scans_remaining<=0)
	{
		IOHandler::coutc("No more scans available\n", IOHandler::LIGHTRED);
		return ;
	}
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	int region = IOHandler::toInt(word_list[1]);
	Region *current_region = NULL;
	for(int i=0;i<row_size;i++)
	{
		for(int j=0;j<col_size;j++)
		{
			Region *r = region_list[i][j];
			if(r->getId()==region)
			{
				current_region=r; break;
			}
		}
	}
	if(current_region==NULL)
	{
		IOHandler::coutc("Invalid region ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	scans_remaining--;
	current_region->scan(*this);
}

void Game::lockdown(const vector<string> &word_list)
{
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	int region = IOHandler::toInt(word_list[1]);
	if(region<0) //state lockdown
	{
		if(state_lockdown_remaining<=0)
		{
			IOHandler::coutc("State lockdown toggle unavailable.\n", IOHandler::LIGHTRED);
			return ;
		}
		if(word_list[1].length()!=1)
		{
			displayToolTip(word_list[0]); 
			return ;
		}
		char c = word_list[1][0];
		State *current_state = NULL;
		for(State *t:state_list)
		{
			if(tolower(t->getId())==c)
			{
				current_state=t; break;
			}
		}
		if(current_state==NULL)
		{
			IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
			return ;
		}
		current_state->setLockDown(true);
		cout<<"State "<<char(toupper(c))<<" is now under lockdown.\n";
		state_lockdown_remaining--;
	}
	else
	{
		if(region_lockdown_remaining<=0)
		{
			IOHandler::coutc("Region lockdown toggle unavailable.\n", IOHandler::LIGHTRED);
			return ;
		}
		Region *current_region = NULL;
		for(int i=0;i<row_size;i++)
		{
			for(int j=0;j<col_size;j++)
			{
				Region *r = region_list[i][j];
				if(r->getId()==region)
				{
					current_region=r; break;
				}
			}
		}
		if(current_region==NULL)
		{
			IOHandler::coutc("Invalid region ID.\n", IOHandler::LIGHTRED);
			return ;
		}
		current_region->setLockDown(true);
		cout<<"Region "<<region<<" is now under lockdown.\n";
		region_lockdown_remaining--;
	}
}

void Game::unlockdown(const vector<string> &word_list)
{
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	int region = IOHandler::toInt(word_list[1]);
	if(region<0) //state lockdown
	{
		if(state_lockdown_remaining<=0)
		{
			IOHandler::coutc("State lockdown toggle unavailable.\n", IOHandler::LIGHTRED);
			return ;
		}
		if(word_list[1].length()!=1)
		{
			displayToolTip(word_list[0]); 
			return ;
		}
		char c = word_list[1][0];
		State *current_state = NULL;
		for(State *t:state_list)
		{
			if(tolower(t->getId())==c)
			{
				current_state=t; break;
			}
		}
		if(current_state==NULL)
		{
			IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
			return ;
		}
		current_state->setLockDown(false);
		cout<<"State "<<char(toupper(c))<<" is now not under lockdown.\n";
		state_lockdown_remaining--;
	}
	else
	{
		if(region_lockdown_remaining<=0)
		{
			IOHandler::coutc("Region lockdown toggle unavailable.\n", IOHandler::LIGHTRED);
			return ;
		}
		Region *current_region = NULL;
		for(int i=0;i<row_size;i++)
		{
			for(int j=0;j<col_size;j++)
			{
				Region *r = region_list[i][j];
				if(r->getId()==region)
				{
					current_region=r; break;
				}
			}
		}
		if(current_region==NULL)
		{
			IOHandler::coutc("Invalid region ID.\n", IOHandler::LIGHTRED);
			return ;
		}
		current_region->setLockDown(false);
		cout<<"Region "<<region<<" is now not under lockdown.\n";
		region_lockdown_remaining--;
	}
}

void Game::movementControl(const vector<string> &word_list)
{
	if(state_movement_control_remaining<=0)
	{
		IOHandler::coutc("Movement control unavailable.\n", IOHandler::LIGHTRED);
		return ;
	}
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	if(word_list[1].length()!=1)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	char c = word_list[1][0];
	State *current_state = NULL;
	for(State *t:state_list)
	{
		if(tolower(t->getId())==c)
		{
			current_state=t; break;
		}
	}
	if(current_state==NULL)
	{
		IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	current_state->stateMovementControl(true);
	cout<<"State "<<char(toupper(c))<<" is now under movement control.\n";
	state_movement_control_remaining--;
}

void Game::unmovementControl(const vector<string> &word_list)
{
	if(word_list.size()!=2)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	if(word_list[1].length()!=1)
	{
		displayToolTip(word_list[0]); 
		return ;
	}
	char c = word_list[1][0];
	State *current_state = NULL;
	for(State *t:state_list)
	{
		if(tolower(t->getId())==c)
		{
			current_state=t; break;
		}
	}
	if(current_state==NULL)
	{
		IOHandler::coutc("Invalid state ID.\n", IOHandler::LIGHTRED);
		return ;
	}
	current_state->stateMovementControl(false);
	cout<<"State "<<char(toupper(c))<<" is now not under movement control.\n";
}

int Game::upgradeAccuracyMax()
{
	int current_percentage = round(testing_accuracy*100.0);
	long long current_cost = getAccuracyUpgradeCost(current_percentage);
	for(int i=100;i>=current_percentage;i--)
	{
		if(getAccuracyUpgradeCost(i)-current_cost<=money)
		{
			return i-current_percentage;
		}
	}
	return 0;
}

long long Game::getAccuracyUpgradeCost(int level)
{
	return (long long)(pow(1.045,double(level))*double(9000)); //1.045^lvl*9000
}

int Game::upgradeScanMax()
{
	int current_level = scan_limit;
	int max_level = MAX_SCAN_LIMIT;
	long long current_cost = getScanLimitUpgradeCost(current_level);
	while(getScanLimitUpgradeCost(max_level)-current_cost>money)
	{
		max_level--;
	}
	assert(max_level>=current_level);
	return max_level-current_level;
}

long long Game::getScanLimitUpgradeCost(int level)
{
	return (long long)(1.49*level*level+1000.0*level);
}

int Game::upgradeTestMax(const State *s)
{
	return min(1LL*testing_kit_limit,money/TESTING_KIT_COST);
}

int Game::upgradeMedicalMax(const State *s)
{
	return min(1LL*medical_capacity_limit,money/MEDICAL_CAPACITY_COST);
}
	
int Game::ignoreMax(const State *s)
{
	return min(s->getHospitalizedCount(),patient_ignore_remaining);
}

int Game::getMaxPatientIgnore(const Time &t)
{
	int cycle_count = t.toHours()/PATIENT_IGNORE_LIMIT_RECHARGE_TIME;
	return int(9.4*pow(double(cycle_count),1.6));
}

int Game::getMaxMedicalCapacityUpgrade(const Time &t)
{
	int cycle_count = t.toHours()/MEDICAL_CAPACITY_RECHARGE_TIME;
	return int(4.0*pow(double(cycle_count),1.35) + 15.0*cycle_count);
}

int Game::getMaxTestingKitUpgrade(const Time &t)
{
	int cycle_count = t.toHours()/TESTING_KIT_RECHARGE_TIME;
	return int(10.0*pow(double(cycle_count),1.35) + 30.0*cycle_count);
}
	
void Game::upgradeAccuracyToolTip(const vector<string> &word_list)
{
	cout<<"You can upgrade test accuracy by at most "<<upgradeAccuracyMax()<<"%.\n";
	if(testing_accuracy<1.0-1e-10) cout<<"Cost to upgrade by 1%: $"<<getAccuracyUpgradeCost(round(testing_accuracy*100.0)+1)-getAccuracyUpgradeCost(round(testing_accuracy*100.0))<<'\n';
}
	
void Game::upgradeScanToolTip(const vector<string> &word_list)
{
	cout<<"You can increase scan limit by at most "<<upgradeScanMax()<<".\n";
	if(scan_limit<MAX_SCAN_LIMIT) cout<<"Cost to increase scan limit by 1: $"<<getScanLimitUpgradeCost(scan_limit+1)-getScanLimitUpgradeCost(scan_limit)<<'\n';
}
	
void Game::upgradeTestToolTip(const vector<string> &word_list)
{
	cout<<"You can purchase at most "<<upgradeTestMax(state_list[0])<<" testing kits.\n";
	cout<<"Cost per testing kit = $"<<TESTING_KIT_COST<<'\n';
}

void Game::upgradeMedicalToolTip(const vector<string> &word_list)
{
	cout<<"You can increase medical capacity by at most "<<upgradeMedicalMax(state_list[0])<<".\n";
	cout<<"Cost per medical capacity = $"<<MEDICAL_CAPACITY_COST<<'\n';
}

void Game::ignoreToolTip(const vector<string> &word_list)
{
	cout<<"You can ignore at most "<<patient_ignore_remaining<<" patients in total.\n";
}

void Game::scanToolTip(const vector<string> &word_list)
{
	cout<<"Number of scans remaining: "<<scans_remaining<<'\n';
	cout<<"Number of tests per scan: "<<scan_limit<<'\n';
}

void Game::lockdownToolTip(const vector<string> &word_list)
{
	cout<<"Number of state lockdown toggles remaining: "<<state_lockdown_remaining<<'\n';
	cout<<"Number of region lockdown toggles remaining: "<<region_lockdown_remaining<<'\n';
}

void Game::unlockdownToolTip(const vector<string> &word_list)
{
	cout<<"Number of state lockdown toggles remaining: "<<state_lockdown_remaining<<'\n';
	cout<<"Number of region lockdown toggles remaining: "<<region_lockdown_remaining<<'\n';
}

void Game::movementControlToolTip(const vector<string> &word_list)
{
	vector<State*> not_movement_control_states;
	for(State *s:state_list)
	{
		if(s->isMovementControl()) continue;
		not_movement_control_states.push_back(s);
	}
	if(not_movement_control_states.empty()) cout<<"Every state is under movement control.\n";
	else 
	{
		cout<<"List of states not under movement control: ";
		for(State *s:not_movement_control_states) cout<<s->getId()<<' ';
		cout<<'\n';
	}
	cout<<"Number of movement control activations remaining: "<<state_movement_control_remaining<<'\n';
}

void Game::unmovementControlToolTip(const vector<string> &word_list)
{
	vector<State*> movement_control_states;
	for(State *s:state_list)
	{
		if(!s->isMovementControl()) continue;
		movement_control_states.push_back(s);
	}
	if(movement_control_states.empty()) cout<<"No state is under movement control.\n";
	else 
	{
		cout<<"List of states under movement control: ";
		for(State *s:movement_control_states) cout<<s->getId()<<' ';
		cout<<'\n';
	}
}

void Game::displayToolTip(const string &s)
{
	if(command_tooltips.find(s)==command_tooltips.end())
	{
		IOHandler::coutc("Error: Command not found.\n", IOHandler::LIGHTRED);
	}
	else
	{
		IOHandler::coutc(command_tooltips[s]+"\n", IOHandler::LIGHTCYAN);
	}
}

void Game::commandPanel() //when user presses any key, switch to command mode until user exits
{
	if(!kbhit()) return ; //only activates when a key is pressed
	while(true)
	{
		cout<<"Enter command (h to display help, e to exit command panel):\n";
		string s = IOHandler::getInput();
		vector<string> vec = IOHandler::toWordList(s);
		if(vec.size()==1&&(vec[0]=="e"||vec[0]=="exit")) break; //exits
		processCommand(s);
	}
}

void Game::updateHour() //updates performed at the end of hour, call at end of startHour()
{
	vector<Person*> new_active_list; //list of active people
	vector<Person*> new_detected_list; //list of detected people (including hospitalized)
	vector<Person*> new_dead_list; //list of dead people
	//update status of all alive citizens (including economy)
	for(Person* p:active_list) //all active people moves
	{
		p->move(*this); 
	}
	for(Person* p:active_list)
	{
		p->updateHour(*this);
	}
	for(Person* p:detected_list)
	{
		p->updateHour(*this);
	}
	//remove cured patients from state hospitals
	for(State *s:state_list)
	{
		vector<Person*> new_hospital_list;
		int patient_number = s->numberOfPatients();
		for(int i=0;i<patient_number;i++)
		{
			Person *p = s->hospitalAt(i);
			if(p->isHealthy()) s->addActivePerson(p);
			else if(p->isDead()) continue; //dead person gets removed as well
			else new_hospital_list.push_back(p);
		}
		//remove cured patients
		s->updateHospitalizedPeople(new_hospital_list);
	}
	//perform testing first, to update infected status
	if(isTestTime())
	{
		for(State *s:state_list) s->test(*this);
	}
	//update new list of people based on their status
	for(Person* p:active_list)
	{
		if(p->isDead()) new_dead_list.push_back(p);
		else if(p->isDetected()) new_detected_list.push_back(p);
		else new_active_list.push_back(p);
	}
	for(Person* p:detected_list)
	{
		if(p->isDead()) new_dead_list.push_back(p);
		else if(p->isDetected()) new_detected_list.push_back(p);
		else new_active_list.push_back(p);
	}
	for(Person* p:dead_list)
	{
		new_dead_list.push_back(p);
	}
	active_list = new_active_list; 
	detected_list = new_detected_list;
	dead_list = new_dead_list;
	double hour_sum=0; //sum of money in this hour
	for(Person* p:active_list)
	{
		hour_sum+=p->getIncome();
	}
	money+=round(hour_sum);
	//for(Person* p:active_list) assert(!(p->isDetected()));
	//for(Person* p:detected_list) assert(p->isDetected());
	//for(Person* p:dead_list) assert(p->isDead()); 
	srand(time(NULL));
	random_shuffle(active_list.begin(),active_list.end());
	random_shuffle(detected_list.begin(),detected_list.end());
	//update state lists from old list 
	for(State *s:state_list)
	{
		s->clearActivePeople(); s->clearDetectedPeople();
	}
	for(Person *p:detected_list)
	{
		State *s = p->getState(*this);
		s->addDetectedPerson(p); //add person as detected anyway
		assert(p->isDetected());
		if(p->isHospitalized()) continue;
		if(!s->isFull()) s->addHospitalizedPerson(p);
	}
	for(Person *p:active_list)
	{
		State *s = p->getState(*this);
		s->addActivePerson(p);
	}
	//update active people in regions
	for(State *s:state_list)
	{
		vector<Region*> region_list = s->getRegionList();
		for(Region *r:region_list)
		{
			r->clearActivePeople();
		}
		vector<Person*> old_active_list = s->getActiveList();
		for(Person *p:old_active_list)
		{
			Region *r = p->getRegion(*this);
			r->addActivePerson(p);
		}
	}
	//list update complete
	resetBoard(); //clear board
	for(Person *p:active_list) //if discovered, sent to quarantine so won't infect area
	{
		if(!p->isHealthy()) stepOn(p->getPosX(), p->getPosY()); //infect area
	}
	game_time.increase(); //update game time
	//update game constants
	if(game_time.toHours()%SCAN_RECHARGE_TIME==0)
	{
		scans_remaining=MAX_SCANS;
	}
	if(game_time.toHours()%STATE_LOCKDOWN_RECHARGE_TIME==0)
	{
		state_lockdown_remaining=MAX_STATE_LOCKDOWN;
	}
	if(game_time.toHours()%REGION_LOCKDOWN_RECHARGE_TIME==0)
	{
		region_lockdown_remaining=MAX_REGION_LOCKDOWN;
	}
	if(game_time.toHours()%MOVEMENT_CONTROL_RECHARGE_TIME==0)
	{
		state_movement_control_remaining=MAX_MOVEMENT_CONTROL;
	}
	if(game_time.toHours()%PATIENT_IGNORE_LIMIT_RECHARGE_TIME==0)
	{
		patient_ignore_remaining=getMaxPatientIgnore(game_time);
	}
	if(game_time.toHours()%TESTING_KIT_RECHARGE_TIME==0)
	{
		testing_kit_limit=getMaxTestingKitUpgrade(game_time);
	}
	if(game_time.toHours()%MEDICAL_CAPACITY_RECHARGE_TIME==0)
	{
		medical_capacity_limit=getMaxMedicalCapacityUpgrade(game_time);
	}
}

void Game::displayMap() //displays the map
{
	for(int i=0;i<=row_size;i++)
	{
		//draw horizontal lines
		for(int j=0;j<=col_size;j++)
		{
			if(i>0) 
			{
				IOHandler::colors text_color = IOHandler::WHITE;
				if(isValidRegion(i-1,j-1)&&region_list[i-1][j-1]->isLockDown()) text_color=IOHandler::LIGHTRED;
				if(isValidRegion(i-1,j)&&region_list[i-1][j]->isLockDown()) text_color=IOHandler::LIGHTRED;
				if(isValidRegion(i-1,j)&&isValidRegion(i-1,j-1))
				{
					State *s1 = region_list[i-1][j]->getState();
					State *s2 = region_list[i-1][j-1]->getState();
					if(s1->getId()!=s2->getId())
					{
						if(s1->isLockDown()||s2->isLockDown()) text_color=IOHandler::LIGHTRED;
					}
				}
				else if(isValidRegion(i-1,j))
				{
					State *s = region_list[i-1][j]->getState();
					if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
				else if(isValidRegion(i-1,j-1))
				{
					State *s = region_list[i-1][j-1]->getState();
					if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
				IOHandler::coutc("|",text_color);
			}
			else cout<<" ";
			if(j==col_size) break;
			{
				IOHandler::colors text_color = IOHandler::WHITE;
				if(isValidRegion(i-1,j)&&region_list[i-1][j]->isLockDown()) text_color=IOHandler::LIGHTRED;
				if(isValidRegion(i,j)&&region_list[i][j]->isLockDown()) text_color=IOHandler::LIGHTRED;
				if(isValidRegion(i,j)&&isValidRegion(i-1,j))
				{
					State *s1 = region_list[i][j]->getState();
					State *s2 = region_list[i-1][j]->getState();
					if(s1->getId()!=s2->getId())
					{
						if(s1->isLockDown()||s2->isLockDown()) text_color=IOHandler::LIGHTRED;
					}
				}
				else if(isValidRegion(i,j))
				{
					State *s = region_list[i][j]->getState();
					if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
				else if(isValidRegion(i-1,j))
				{
					State *s = region_list[i-1][j]->getState();
					if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
				IOHandler::coutc("__",text_color); //change color if lockdown
			}
		}
		cout<<"\n";
		if(i==row_size) break; //end
		for(int j=0;j<=col_size;j++)
		{
			IOHandler::colors text_color = IOHandler::WHITE;
			if(isValidRegion(i,j-1)&&region_list[i][j-1]->isLockDown()) text_color=IOHandler::LIGHTRED;
			if(isValidRegion(i,j)&&region_list[i][j]->isLockDown()) text_color=IOHandler::LIGHTRED;
			if(isValidRegion(i,j)&&isValidRegion(i,j-1))
			{
				State *s1 = region_list[i][j]->getState();
				State *s2 = region_list[i][j-1]->getState();
				if(s1->getId()!=s2->getId())
				{
					if(s1->isLockDown()||s2->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
			}
			else if(isValidRegion(i,j))
			{
				State *s = region_list[i][j]->getState();
				if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
			}
			else if(isValidRegion(i,j-1))
			{
				State *s = region_list[i][j-1]->getState();
				if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
			}
			IOHandler::coutc("|",text_color); //change color if lockdown
			if(j==col_size) break;
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			if(region_list[i][j]->getState()->isMovementControl())
			{
				SetConsoleTextAttribute(handle, IOHandler::RED);
			}
			else if(region_list[i][j]->getState()->isLockDown())
			{
				SetConsoleTextAttribute(handle, IOHandler::LIGHTRED);
			}
			cout<<setw(2)<<region_list[i][j]->getState()->getId();
			SetConsoleTextAttribute(handle, IOHandler::WHITE);
		}
		cout<<"\n";
		for(int j=0;j<=col_size;j++)
		{
			IOHandler::colors text_color = IOHandler::WHITE;
			if(isValidRegion(i,j-1)&&region_list[i][j-1]->isLockDown()) text_color=IOHandler::LIGHTRED;
			if(isValidRegion(i,j)&&region_list[i][j]->isLockDown()) text_color=IOHandler::LIGHTRED;
			if(isValidRegion(i,j)&&isValidRegion(i,j-1))
			{
				State *s1 = region_list[i][j]->getState();
				State *s2 = region_list[i][j-1]->getState();
				if(s1->getId()!=s2->getId())
				{
					if(s1->isLockDown()||s2->isLockDown()) text_color=IOHandler::LIGHTRED;
				}
			}
			else if(isValidRegion(i,j))
			{
				State *s = region_list[i][j]->getState();
				if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
			}
			else if(isValidRegion(i,j-1))
			{
				State *s = region_list[i][j-1]->getState();
				if(s->isLockDown()) text_color=IOHandler::LIGHTRED;
			}
			IOHandler::coutc("|",text_color); //change color if lockdown
			if(j==col_size) break;
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			if(region_list[i][j]->isLockDown())
			{
				SetConsoleTextAttribute(handle, IOHandler::LIGHTRED);
			}
			cout<<setw(2)<<region_list[i][j]->getId();
			SetConsoleTextAttribute(handle, IOHandler::WHITE);
		}
		cout<<"\n";
	}
}

void Game::displayStats() //displays the current stats
{
	//include current time, time to end etc
	game_time.displayTime();
	if(time_limit.getHour()<int(1e7))
	{
		Time remaining_time = Time::calcRemaining(game_time,time_limit);
		if(remaining_time.getDay()>=1) 
		{
			cout<<remaining_time.getDay()<<" day"<<(remaining_time.getDay()!=1?"s":"");
			if(remaining_time.getDay()==0||remaining_time.getHour()>=1) cout<<", ";
		}
		if(remaining_time.getDay()==0||remaining_time.getHour()>=1) cout<<remaining_time.getHour()<<" hour"<<(remaining_time.getHour()!=1?"s ":" ");
		else cout<<" ";
		cout<<"remaining.\n";
	}
	if(dead_limit<int(1e7))
	{
		int rem_deaths = dead_limit - int(dead_list.size());
		assert(rem_deaths>0);
		if(rem_deaths>1) cout<<rem_deaths<<" more deaths before game over.\n";
		else cout<<rem_deaths<<" more death before game over.\n";
	}
	int infected_count=0;
	for(Person *p:active_list)
	{
		if(!p->isHealthy()) infected_count++;
	}
	for(Person *p:detected_list)
	{
		assert(!p->isHealthy());
		infected_count++;
	}
	cout<<'\n';
	cout<<"Budget: $"<<money<<'\n';
	cout<<"Infected count: "<<infected_count<<'\n';
	cout<<"Active people: "<<active_list.size()<<'\n';
	cout<<"Detected people: "<<detected_list.size()<<'\n';
	cout<<"Dead people: "<<dead_list.size()<<'\n';
	cout<<"Testing accuracy: "<<floor(double(testing_accuracy*100))<<"%\n";
	cout<<"Scans remaining: "<<scans_remaining<<"/"<<MAX_SCANS<<" (recharges every "<<SCAN_RECHARGE_TIME<<" hours)\n";
	cout<<"Max tests per scan: "<<scan_limit<<'\n';
	cout<<"State lockdown toggles remaining: "<<state_lockdown_remaining<<"/"<<MAX_STATE_LOCKDOWN<<" (recharges every "<<STATE_LOCKDOWN_RECHARGE_TIME<<" hours)\n";
	cout<<"Region lockdown toggles remaining: "<<region_lockdown_remaining<<"/"<<MAX_REGION_LOCKDOWN<<" (recharges every "<<REGION_LOCKDOWN_RECHARGE_TIME<<" hours)\n";
	cout<<"State movement control remaining: "<<state_movement_control_remaining<<"/"<<MAX_MOVEMENT_CONTROL<<" (recharges every "<<MOVEMENT_CONTROL_RECHARGE_TIME<<" hours)\n";
	cout<<"You have ignored "<<getMaxPatientIgnore(game_time)-patient_ignore_remaining<<"/"<<getMaxPatientIgnore(game_time)<<" patients. (resets every "<<PATIENT_IGNORE_LIMIT_RECHARGE_TIME<<" hours)\n";
	cout<<"Number of testing kit upgrades remaining: "<<testing_kit_limit<<"/"<<getMaxTestingKitUpgrade(game_time)<<" (resets every "<<TESTING_KIT_RECHARGE_TIME<<" hours)\n";
	cout<<"Number of medical capacity upgrades remaining: "<<medical_capacity_limit<<"/"<<getMaxMedicalCapacityUpgrade(game_time)<<" (resets every "<<MEDICAL_CAPACITY_RECHARGE_TIME<<" hours)\n";
	cout<<'\n';
	cout<<"State Stats:\n";
	for(State *s:state_list)
	{
		cout<<"State "<<s->getId()<<": ";
		cout<<"Active People: "<<right<<setw(6)<<s->getActiveCount()<<"/"<<left<<setw(6)<<s->getActiveCount()+s->getDetectedCount()<<"      ";
		cout<<"Detected People: "<<right<<setw(6)<<s->getDetectedCount()<<"/"<<left<<setw(6)<<s->getActiveCount()+s->getDetectedCount()<<"      ";
		cout<<"Hospitalized People: "<<right<<setw(6)<<s->getHospitalizedCount()<<"/"<<left<<setw(6)<<s->getMedicalCapacity()<<"      ";
		cout<<"Testing Cap: "<<right<<setw(6)<<s->getTestingKits();
		cout<<'\n';
	}
}

void Game::startHour() //starts a new hour (displays new map, stats and etc)
{
	system("cls"); //clear screen
	clock_t st = clock();
	displayMap();
	cout<<"\n\n";
	displayStats(); 
	clock_t ed = clock();
	double time_elapsed = double(ed-st)/double(CLOCKS_PER_SEC);
	double diff = getMinSecondsBetweenHour()-time_elapsed;
	if(diff>=0) Sleep(diff*1000.0); //in miliseconds
	commandPanel(); //check if user uses command panel
	updateHour();
	int win_condition = checkWinCondition();
	if(win_condition!=0)
	{
		endGame(win_condition);
	}
}

int Game::checkWinCondition() //check if the current state ends the game, -1 if lose, 1 if win, 0 if not end
{
	int ex_sick = 0;
	for(Person* p:active_list)
	{
		if(p->isHospitalized()) continue;
		if(p->isHealthy()) continue;
		ex_sick = 1;
	}
	for(Person* p:detected_list)
	{
		if(p->isHospitalized()) continue;
		if(p->isHealthy()) continue;
		ex_sick = 1;
	}
	if(!ex_sick) return 1; //win if no sick
	if(int(dead_list.size())>=dead_limit) return -1; //too many deaths
	if(time_limit<game_time) return -1; //over time limit
	return 0; //game continues
}

void Game::endGame(int win_condition) //ends the game and display the user's results
{
	system("cls");
	if(win_condition>0) 
	{
		IOHandler::coutc("Congratulations! You won!\n", IOHandler::LIGHTGREEN);
	}
	else
	{
		IOHandler::coutc("You lost to the virus :( Better luck next time!\n", IOHandler::LIGHTRED);
	}
	//display game stats
	cout<<"Number of citizens alive: "<<active_list.size()<<'\n';
	cout<<"Number of citizens dead: "<<dead_list.size()<<'\n';
	cout<<"Death percentage: "<<fixed<<setprecision(2)<<double(dead_list.size()*100)/double(dead_list.size()+active_list.size()+detected_list.size())<<"%"<<'\n';
	cout<<"Time elapsed: ";
	if(game_time.getDay()>=1) 
	{
		cout<<game_time.getDay()<<" day"<<(game_time.getDay()!=1?"s":"");
		if(game_time.getDay()==0||game_time.getHour()>=1) cout<<", ";
	}
	if(game_time.getDay()==0||game_time.getHour()>=1) cout<<game_time.getHour()<<" hour"<<(game_time.getHour()!=1?"s ":" ");
	else cout<<" ";
	cout<<'\n';
	system("pause");
}

bool Game::isTestTime()
{
	return (game_time.toHours()%HOURS_BETWEEN_TESTS==0);
}

int Game::getWanderRange(Person *p)
{
	if(p->getState(*this)->isMovementControl()) return 3;
	else if(p->getRegion(*this)->isLockDown()) return 5;
	else if(p->getState(*this)->isLockDown()) return 7;
	return 11;
}

int Game::getMerchantWorkRange(Merchant *m)
{
	return WORK_RADIUS*2;
}

double Game::getInfectionProbability(int visits)
{
	double spread_mult = virus.getSpreadMultiplier();
	return max(0.0,min(1.0,spread_mult*double(visits))); //subject to change
}


