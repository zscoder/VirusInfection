/*
 * Class that implements gametime-related functions
 */

#include "Random.h"

#ifndef TIME_H
#define TIME_H //header guards
class Time
{
	private:
	static const int HOURS_PER_DAY = 24;
	static const int DAYS_PER_WEEK = 7;
	const string DAY_NAMES[DAYS_PER_WEEK] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
	int day,hour;
	public:
	Time(int _day=0, int _hour=0): day(_day),hour(_hour){};
	//Basic Functions
	int getDay() const {return day;}
	bool isWeekday() const {return (day%DAYS_PER_WEEK>=1&&day%DAYS_PER_WEEK<=5);}
	bool isWeekend() const {return (!isWeekday());}
	int getHour() const {return hour;}
	void increase(); //add 1 hour
	void setDay(int _day) {day=_day;}
	int toHours() const {return HOURS_PER_DAY*day+hour;}
	void setHour(int _hour) {hour=_hour;}
	void const displayTime(); //display current time
	static Time calcRemaining(const Time &curtime, const Time &endtime); 
	//Time operators
	bool operator < (const Time &r) //is current time less than compared time?
	{
		if(day!=r.day) return day<r.day;
		return hour<r.hour;
	} 
	Time& operator = (const Time &t) //assignment operator
	{
		day=t.day; hour=t.hour; return *this;
	} 
	bool operator <= (const Time &r) //is current time less than or equal to compared time?
	{
		if(day!=r.day) return day<=r.day;
		return hour<=r.hour;
	} 
};
#endif
