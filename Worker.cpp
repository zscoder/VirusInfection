#include "Worker.h"
#include "Game.h"
#include "Region.h" 
#include "Time.h"

void Worker::moveWork(Game &game) 
{
	moveShortestPath(job_x,job_y,game);
}

void Worker::wander(Game &game)
{
	int range = game.getWanderRange(this);
	Region *r = getRegion(game);
	int mnx = game.AREA_PER_GRID*r->getX();
	int mxx = mnx + game.AREA_PER_GRID - 1;
	int mny = game.AREA_PER_GRID*r->getY();
	int mxy = mny + game.AREA_PER_GRID - 1;
	for(int i=0;i<activity_level;i++)
	{
		int nwx = Random::getRand(max(mnx,curx-range),min(mxx,curx+range));
		int nwy = Random::getRand(max(mny,cury-range),min(mxy,cury+range));
		moveShortestPath(nwx,nwy,game);
	}
}

void Worker::move(Game &game)
{
	if(!isFit()) return ;
	if(isDetected()) return ; //detected or unhealthy => no movement
	Time t = game.getGameTime();
	if(t.isWeekend()||!is_working)
	{
		if(Random::getDecision(0.5)) wander(game); //wander with 1/2 chance
	}
	else
	{
		//work 
		int hr = t.getHour();
		if(hr<work_start_hr)
		{
			
		}
		else if(hr==work_start_hr) //move to work
		{
			moveWork(game);
		}
		else if(hr>work_start_hr&&hr<work_end_hr)
		{
			
		}
		else if(hr==work_end_hr) //move home
		{
			moveShortestPath(home_x,home_y,game);
		}
		else if(hr==23) //must return home before midnight
		{
			moveShortestPath(home_x,home_y,game);
		}
		else
		{
			if(Random::getDecision(0.5)) wander(game);
		}
	}
}

void Worker::updateHour(Game &game)
{
	Person::updateHour(game);
	//update is_working variable
	is_working = 1;
	if(!isFit()) {is_working = 0;}
	else //suppose person is fit, then check lockdown status
	{
		if(getRegion(game)->isLockDown()) is_working=0;
	}
}

double Worker::getIncome()
{
	return (is_working?salary_per_hour:0.0);
}
