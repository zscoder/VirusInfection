/*
 * A class that handles a state in the game
 */

#include "Random.h"
#include "Person.h"
#include "Region.h"

#ifndef STATE_H
#define STATE_H //header guards

class Game; //forward declaration

class State
{
	private:
	vector<Region*> region_list; //list of regions in this state
	vector<Person*> active_people; //list of active people in this state
	vector<Person*> detected_people; //list of detected people in this state
	vector<Person*> hospitalized_people; //list of hospitalized people in this state
	int testing_kits; //number of testing kits
	int medical_capacity; //number of patients the state can hold
	char id; //id of state
	bool lockdown; //is the state under lockdown?
	bool movement_control; //is the state under movement control
	int undetected_hours = 0;
	public:
	State(int testing_kits, int medical_capacity, char id, bool lockdown=0, bool movement_control=0): testing_kits(testing_kits), medical_capacity(medical_capacity), id(id), lockdown(lockdown), movement_control(movement_control){};
	
	//Getter Functions
	bool isLockDown() const {return lockdown;}
	bool isMovementControl() const {return movement_control;}
	bool isFull() const {return (int(hospitalized_people.size())>=medical_capacity);}
	void setLockDown(bool val) {lockdown = val;} //set lockdown value
	int getTestingKits() const {return testing_kits;}
	int getMedicalCapacity() const {return medical_capacity;}
	int getStateSize() const {return region_list.size();}
	int getActiveCount() const {return active_people.size();}
	int getDetectedCount() const {return detected_people.size();}
	int getHospitalizedCount() const {return hospitalized_people.size();}
	int getUndetectedHours() const {return undetected_hours;}
	void setUndetectedHours(int val) {undetected_hours = val;}
	char getId() const {return id;}
	vector<Region*> getRegionList() const {return region_list;}
	Person* hospitalAt(int id) const {return hospitalized_people[id];}
	vector<Person*> getDetectedList() const {return detected_people;}
	vector<Person*> getActiveList() const {return active_people;}
	int numberOfPatients() const {return hospitalized_people.size();}	
	
	//Setter Functions
	void addTestingKits(int kit_count) {testing_kits+=kit_count;}
	void addMedicalCap(int cap_add) {medical_capacity+=cap_add;}
	void addRegion(Region *r) {region_list.push_back(r);}
	void ignoreSickest(int k); //abandons k sickest patients in this state
	void stateMovementControl(bool toggle=1); //toggle = 1 => apply movement control, 0 = undo
	void updateHospitalizedPeople(vector<Person*> &vec) {hospitalized_people = vec;} //update hospitalized people
	void addActivePerson(Person *p) {active_people.push_back(p);}
	void addDetectedPerson(Person *p) {detected_people.push_back(p);}
	void addHospitalizedPerson(Person *p); //use this to add hospitalized person!
	void updateDetectedPeople(vector<Person*> &vec) {detected_people = vec;} //update detected people	
	void clearActivePeople() {active_people.clear();}
	void clearDetectedPeople() {detected_people.clear();}
	void updateActivePeople(vector<Person*> &vec) {active_people = vec;} //update active people
	void test(Game &game); //usual testing in the state every few hours
};

#endif


