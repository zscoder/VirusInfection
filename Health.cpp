#include "Health.h"

void Health::infect(const Virus &v) //infected with virus v
{
	assert(healthy); //for simplicity, we can only infect healthy people
	healthy = 0;
	assert(v.getMinIncubationPeriod()>0); //incubation period must be positive
	sick_hours = -Random::getRand(v.getMinIncubationPeriod(),v.getMaxIncubationPeriod());
	assert(v.getMinLethality()<=v.getMaxLethality());
	lethal_hours = Random::getRand(v.getMinLethality(),v.getMaxLethality());
	cure_hours = v.getMinCureHours(); //minimum # of hours needed to cure the virus
	cure_difficulty = v.getCureDifficulty();
}

void Health::update() //update status after one hour
{
	if(healthy) return ; 
	if(lethal_hours>=INF) //currently curing
	{
		cure_hours--;
		if(cure_hours<0)
		{
			healthy=1;
			sick_hours=0;
			lethal_hours=0;
			cure_hours=0; 
			cure_difficulty=0;
			//healed
		}
	}
	else
	{
		sick_hours++;
	}
}

void Health::cure() //change patient to hospitalized mode
{
	sick_hours=max(sick_hours,0);
	cure_hours+=(int)(cure_difficulty*double(sick_hours)); //start to cure
	lethal_hours=INF;
	sick_hours=0;
}
