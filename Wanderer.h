/*
 * A class that handles a type of person in our game: Wanderer
 */

#include "Person.h"

#ifndef WANDERER_H
#define WANDERER_H //header guards

class Game; //forward declaration

class Wanderer : public Person
{
	public:
	Wanderer(int posx, int posy, int activity_level, int id, bool detected=0): Person(posx,posy,activity_level,id,detected){};
	void moveWork(Game &game) override; //perform random moves during work period based on game states
	void move(Game &game) override; //move pattern based on game state
	double getIncome() override; //get income of person, depending on the derived class
};

#endif
