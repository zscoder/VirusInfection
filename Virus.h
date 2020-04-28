/*
 * A class that handles the virus in our game
 */
 
#include "Random.h"

#ifndef VIRUS_H
#define VIRUS_H //header guards

class Virus
{
	private:
	int min_incubation_period; //number of hours before person starts to fall sick
	int max_incubation_period;
	int min_lethality; 
	int max_lethality; //min and max number of hours needed before person dies after falling sick from this virus
	double cure_difficulty; //cure difficulty
	int min_cure_hours; //min # of hours to cure this virus
	double spread_multiplier; //multiplier denoting how easy the virus spreads
	public:	
	Virus(int mininc=0, int maxinc=0, int mn=0, int mx=0, double curediff=0, int curehr=0, double spread_multiplier=0): min_incubation_period(mininc), max_incubation_period(maxinc), min_lethality(mn), max_lethality(mx), cure_difficulty(curediff), min_cure_hours(curehr), spread_multiplier(spread_multiplier){};
	int getMinIncubationPeriod() const {return min_incubation_period;}
	int getMaxIncubationPeriod() const {return max_incubation_period;} 
	int getMinLethality() const {return min_lethality;}  
	int getMaxLethality()const {return max_lethality;} 
	double getCureDifficulty() const {return cure_difficulty;} 
	int getMinCureHours() const {return min_cure_hours;} 
	double getSpreadMultiplier() const {return spread_multiplier;}
};

#endif
