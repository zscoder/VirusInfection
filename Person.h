#include "Random.h"
#include "Health.h"

#ifndef PERSON_H
#define PERSON_H //header guards

class Game; //forward declaration
class State;
class Region;

class Person
{
	protected: //accessible for Merchant, Worker and Wanderer classes
	Health health_state;
	int posx, posy; //position before hour ends
	int curx, cury; //active position while moving, will be updated as posx, posy after hour ends
	string visited; //visited positions in the past hour, compressed as a string
	int activity_level; //activity level, how "far" this guy moves
	int id; //unique id of the person
	bool detected; //detected as virus carrier
	static const int dx[4];
	static const int dy[4];
	static const string directions;
	static const map<pair<int,int>,int> dirguide; //maps a pair vector to a suitable direction
	public:
	Person(int posx, int posy, int activity_level, int id, bool detected=0): posx(posx), posy(posy), activity_level(activity_level), id(id), detected(detected){visited=""; curx=posx; cury=posy;}; //initialized visited array by your own square
	
	int getSign(int x) {return (x<0?-1:1);}
	void moveToPosition(int dir, Game &game);
	void moveShortestPath(int x, int y, Game &game); //move to specific position via random shortest path
	virtual void wander(Game &game); //wander, can cross state unless blocked
	void updateHealth(Game &game); //update health variable by looking at your path this session
	virtual void updateHour(Game &game); //update variables after an hour passed
	void test(Game &game); //tested for virus
	State* getState(Game &game); //returns the state that this person belongs to
	Region* getRegion(Game &game); //returns the region that this person belongs to
	
	virtual void moveWork(Game &game) = 0; //perform random moves during work period based on game states
	virtual void move(Game &game) = 0; //move pattern based on game state
	virtual double getIncome() = 0; //get income of person, depending on the derived class
	virtual ~Person(){}; //virtual destructor
	Health getHealth() const {return health_state;}
	
	bool isDead() const {return health_state.isDead();}
	bool isHealthy() const {return health_state.isHealthy();}
	bool isDetected() const {return detected;}
	bool isHospitalized() const {return health_state.isCuring();}
	bool isFit() const {return ((!detected)&&(health_state.getSickHours()<=0));} //check if person is fit to work
	void infect(const Virus &v){health_state.infect(v);}
	int getPosX() const {return posx;}
	int getPosY() const {return posy;}
	int getCurX() const {return curx;}
	int getCurY() const {return cury;}
	int getActivityLevel() const {return activity_level;}
	int getId() const {return id;}
	void clearVisited(){visited="";}
	string getVisited() const {return visited;}
	void cure() {health_state.cure();}
	void setDead() {health_state.setDead();}
	bool operator < (const Person &r) //sort by cure hours
	{
		return (health_state<r.health_state);
	}
};
#endif
