#include "Time.h"

void Time::increase()
{
	hour++;
	if(hour>=HOURS_PER_DAY) {hour=0; day++;}
}

void const Time::displayTime()
{
	cout<<"Day "<<day<<" ("<<DAY_NAMES[day%DAYS_PER_WEEK]<<")\t"<<"Hour "<<hour<<'\n';
}

Time Time::calcRemaining(const Time &curtime, const Time &endtime)
{
	int t2 = endtime.day*HOURS_PER_DAY+endtime.hour;
	int t1 = curtime.day*HOURS_PER_DAY+curtime.hour;
	t2-=t1;
	assert(t2>=0);
	return Time(t2/HOURS_PER_DAY,t2%HOURS_PER_DAY);
}
