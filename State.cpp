#include "State.h"
#include "Game.h"

void State::ignoreSickest(int k) //ignore k sickest patients
{
	auto sortbyCure = [](Person *p1, Person *p2) -> bool //sort based on cure hours
	{
		return (*p1)<(*p2);
	};
	sort(hospitalized_people.rbegin(),hospitalized_people.rend(),sortbyCure);
	for(int i=0;i<min(k,int(hospitalized_people.size()));i++)
	{
		hospitalized_people[i]->setDead(); //set ignored patients as dead
	}
}

void State::stateMovementControl(bool toggle) //1 if movement control activated, 0 if lifted
{
	for(Region* r:region_list)
	{
		r->setLockDown(toggle);
	}
	movement_control = toggle;
}

void State::addHospitalizedPerson(Person *p)
{
	p->cure(); //start curing this guy
	hospitalized_people.push_back(p);
}

void State::test(Game &game)
{
	vector<Person*> sick,non_sick;
	for(Person* p:active_people)
	{
		if(p->isDead()||p->isDetected()) continue; //don't test bad people, will update in game state later
		if(p->isFit()) non_sick.push_back(p);
		else sick.push_back(p);
	}
	srand(time(NULL));
	random_shuffle(sick.begin(),sick.end()); random_shuffle(non_sick.begin(),non_sick.end());
	int test_count = 0;
	for(Person* p:sick)
	{
		if(test_count<testing_kits)
		{
			p->test(game); test_count++;
		}
	}
	for(Person* p:non_sick)
	{
		if(test_count<testing_kits)
		{
			p->test(game); test_count++;
		}
	}
}
