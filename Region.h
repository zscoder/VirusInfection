/*
 * A class that handles regions in our game.
 */

#include "Random.h"
#include "Person.h"

#ifndef REGION_H
#define REGION_H //header guards

class Game; //forward declaration

class Region
{
	private:
	int x,y; //x and y coordinates on the map
	int id; //id of region
	State* state; //state that this region belongs to
	bool lockdown; //is the region under lockdown?
	vector<Person*> active_people; //list of active people in this region
	public:
	Region(int x, int y, int id, State* state, bool lockdown=0): x(x), y(y), id(id), state(state), lockdown(lockdown){};
	int getX() const {return x;}
	int getY() const {return y;}
	int getId() const {return id;}
	State* getState() const {return state;}
	bool isLockDown() const {return lockdown;}
	void setLockDown(bool val) {lockdown = val;} //set lockdown value
	void scan(Game &game); //scans for people tested positive here
	void addActivePerson(Person* p){active_people.push_back(p);} //add active people
	void clearActivePeople(){active_people.clear();} //clear active people
	void updateActivePeople(vector<Person*> &vec) {active_people = vec;} //update active people
};

#endif

