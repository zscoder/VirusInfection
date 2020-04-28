/*
 * A class that describes one type of person in our map: Merchant
 */

#include "Person.h"

#ifndef MERCHANT_H
#define MERCHANT_H //header guards

class Game; //forward declaration

class Merchant : public Person
{
	private:
	double salary_per_hour; //passive income every hour if working
	bool is_working = 1;
	public:
	Merchant(int posx, int posy, int activity_level, int id, double salary_per_hour=0, bool detected=0): Person(posx,posy,activity_level,id,detected), salary_per_hour(salary_per_hour){};
	void moveWork(Game &game) override; //perform random moves during work period based on game states
	void move(Game &game) override; //move pattern based on game state
	void updateHour(Game &game) override;
	bool isWorking() const {return is_working;}
	double getIncome() override; //get income of person, depending on the derived class
};

#endif
