#include "Region.h"
#include "Game.h"
#include "Person.h"

void Region::scan(Game &game) //scan a region
{
	int scan_limit = game.getScanLimit();
	srand(time(NULL));
	random_shuffle(active_people.begin(),active_people.end());
	int detect_count=0;
	int scans_remaining=scan_limit;
	for(int i=0;i<int(active_people.size());i++) 
	{
		if(scans_remaining<=0) break;
		if(active_people[i]->isDetected()) continue;
		active_people[i]->test(game); scans_remaining--;
		if(active_people[i]->isDetected()) detect_count++;
	}
	cout<<"Number of new virus carriers detected: "<<detect_count<<'\n';
}
