#include "Wanderer.h"
#include "Game.h"

void Wanderer::moveWork(Game &game) //never work, so no implementation
{
	//nothing here
}

void Wanderer::move(Game &game)
{
	if(!isFit()) return ;
	if(isDetected()) return ; //detected or unhealthy => no movement
	if(Random::getDecision(0.5)) wander(game);
}

double Wanderer::getIncome()
{
	return 0;
}
