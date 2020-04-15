#include "Merchant.h"
#include "Game.h"
#include "Region.h"
#include "State.h"

void Merchant::moveWork(Game &game) 
{
	int maxRange = game.getMerchantWorkRange(this);
	int rangex = Random::getRand(max(0,maxRange-20),maxRange);
	int rangey = Random::getRand(max(0,maxRange-20),maxRange);
	int xSize = game.AREA_PER_GRID*game.getRowSize();
	int ySize = game.AREA_PER_GRID*game.getColSize();
	int sgnx = Random::getRand(0,1)*2-1; //-1 or 1
	int sgny = Random::getRand(0,1)*2-1;
	int nwx = max(0,min(xSize-1,sgnx*rangex+curx));
	int nwy = max(0,min(ySize-1,sgny*rangey+cury));
	moveShortestPath(nwx,nwy,game);
}

void Merchant::move(Game &game)
{
	if(!isFit()) return ;
	if(isDetected()) return ; //detected or unhealthy => no movement
	if(is_working)
	{
		int rnd = Random::getRand(0,2);
		if(rnd==0) moveWork(game);
		else if(rnd==1) wander(game);
	}
	else
	{
		if(Random::getDecision(0.5)) wander(game);
	}
}

void Merchant::updateHour(Game &game)
{
	Person::updateHour(game);
	//update is_working variable
	is_working = 1;
	if(!isFit()) {is_working = 0;}
	else //suppose person is fit, then check lockdown status
	{
		if(getRegion(game)->isLockDown()) is_working=0;
		if(getState(game)->isLockDown()) is_working=0;
	}
}

double Merchant::getIncome()
{
	return (is_working?salary_per_hour:0.0);
}


