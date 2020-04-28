/*
 * A class that handles the health state of people in our game
 */

#include "Virus.h"
#include "Random.h"

using namespace std;

#ifndef HEALTH_H
#define HEALTH_H //header guards
class Health
{
	private:
	const int INF = int(1e9);
	bool healthy; //is the person healthy
	int sick_hours; //negative if infected but not sick, # of hours since sick
	int lethal_hours; //if sick_hours >= lethal_hours, person dies
	int cure_hours; //# of hours to cure patient, if +ve other parts stop increasing
	double cure_difficulty; //difficutly to cure patient
	
	public:
	Health(bool _healthy = true, int sick_hr = 0, int lethal_hr = 0, int cure_hr = 0, double cure_diff = 0.0): healthy(_healthy), sick_hours(sick_hr), lethal_hours(lethal_hr), cure_hours(cure_hr), cure_difficulty(cure_diff){};
	void infect(const Virus &v); //infected with virus
	void update(); //update state when hour passed
	void cure(); //switch to cure mode
	void setDead() {sick_hours=INF; lethal_hours=1; healthy=0; cure_hours=0; cure_difficulty=0.0;}
	bool isHealthy() const {return healthy;}
	int getSickHours() const {return sick_hours;}
	int getLethalHours() const {return lethal_hours;}
	int getCureHours() const {return cure_hours;}
	double getCureDifficulty() const {return cure_difficulty;}
	bool isCuring() const {return ((!healthy)&&(lethal_hours>=INF));}
	bool isDead() const {return ((!healthy)&&(sick_hours>=lethal_hours));}
	bool operator < (const Health &h)
	{
		return cure_hours < h.cure_hours;
	}
};

#endif
