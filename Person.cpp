#include "Person.h"
#include "Game.h"
#include "State.h"
#include "Region.h"

const int Person::dx[4] = {1,-1,0,0};
const int Person::dy[4] = {0,0,1,-1};
const string Person::directions = "DURL"; //for reference only
const map<pair<int,int>,int> Person::dirguide = {{{1,0},0},{{-1,0},1},{{0,1},2},{{0,-1},3}};

void Person::moveToPosition(int dir, Game &game) //move to specific position
{
	assert(dir>=0&&dir<=3);
	curx+=dx[dir];
	cury+=dy[dir];
	if(!isHealthy()) game.stepOn(curx,cury);
	visited+='0'+dir; //append current move
}

void Person::moveShortestPath(int x, int y, Game &game) //move to specific position via random shortest path
{
	int path_type = Random::getRand(0,3);
	int diffx = x-curx; int diffy = y-cury;
	int adiffx = abs(diffx); int adiffy = abs(diffy);
	switch(path_type)
	{
		case 0: //x, then y
		{
			int s = getSign(diffx);
			int dir = dirguide.at({s,0});
			for(int i=0;i<adiffx;i++) moveToPosition(dir,game);
		}
		{
			int s = getSign(diffy);
			int dir = dirguide.at({0,s});
			for(int i=0;i<adiffy;i++) moveToPosition(dir,game);
		}
		break;
		case 1: //y, then x
		{
			int s = getSign(diffy);
			int dir = dirguide.at({0,s});
			for(int i=0;i<adiffy;i++) moveToPosition(dir,game);
		}
		{
			int s = getSign(diffx);
			int dir = dirguide.at({s,0});
			for(int i=0;i<adiffx;i++) moveToPosition(dir,game);
		}
		break;
		case 2: //x y alternate
		{
			int dirx = dirguide.at({getSign(diffx),0});
			int diry = dirguide.at({0,getSign(diffy)});
			for(int i=0,j=0;(i<adiffx||j<adiffy);)
			{
				if(i<adiffx)
				{
					moveToPosition(dirx,game);
					i++;
				}
				if(j<adiffy)
				{
					moveToPosition(diry,game);
					j++;
				}
			}
		}
		break;
		case 3: //y x alternate
		{
			int dirx = dirguide.at({getSign(diffx),0});
			int diry = dirguide.at({0,getSign(diffy)});
			for(int i=0,j=0;(i<adiffx||j<adiffy);)
			{
				if(j<adiffy)
				{
					moveToPosition(diry,game);
					j++;
				}
				if(i<adiffx)
				{
					moveToPosition(dirx,game);
					i++;
				}			
			}
		}
		break;
		default:
		assert(0); //impossible
	}
}

void Person::wander(Game &game) //wander, can cross state unless blocked
{
	int range = game.getWanderRange(this);
	int xSize = game.AREA_PER_GRID*game.getRowSize();
	int ySize = game.AREA_PER_GRID*game.getColSize();
	Region *r = getRegion(game); State *s = getState(game);
	bool statelock = s->isLockDown();
	bool regionlock = r->isLockDown();
	if(regionlock)
	{
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
	else if(statelock)
	{
		int successful=0;
		const int C = 5;
		for(int i=0;i<activity_level*C;i++) //try to move activity_level times
		{
			int nwx = Random::getRand(max(0,curx-range),min(xSize-1,curx+range));
			int nwy = Random::getRand(max(0,cury-range),min(ySize-1,cury+range));
			if((game.getState(nwx/game.AREA_PER_GRID,nwy/game.AREA_PER_GRID))->getId()==s->getId()) //if i don't leave the state
			{
				successful++; moveShortestPath(nwx,nwy,game);
			}
			if(successful>=activity_level) break;
		}
	}
	else
	{
		for(int i=0;i<activity_level;i++)
		{
			int nwx = Random::getRand(max(0,curx-range),min(xSize-1,curx+range));
			int nwy = Random::getRand(max(0,cury-range),min(ySize-1,cury+range));
			moveShortestPath(nwx,nwy,game);
		}
	}
}

void Person::updateHealth(Game &game) //update health variable by looking at your path this session
{
	if(!isHealthy())
	{
		health_state.update(); //update health state
		if(isHealthy()){detected=false;} //back to normal!
		return ;
	}
	int x=posx; int y=posy;
	int isInfected = (isHealthy()?0:1);
	int sum=game.getBoardCount(x,y)-isInfected;
	for(int i=0;i<int(visited.length());i++)
	{
		int d = visited[i]-'0'; 
		x+=dx[d]; y+=dy[d];
		sum+=game.getBoardCount(x,y)-isInfected;
	}
	double prob = game.getInfectionProbability(sum);
	if(Random::getDecision(prob)) infect(game.getVirus());
}
	
void Person::updateHour(Game &game) //update variables after an hour passed
{
	updateHealth(game); //update health status
	posx=curx; posy=cury;
	visited="";
}

void Person::test(Game &game) //person gets tested for virus
{
	if(isHealthy()) return ; //nothing since this person is healthy
	if(Random::getDecision(game.getTestingAccuracy()))
	{
		detected=1; //update at game function later
	}
}

State* Person::getState(Game &game) //returns the state that this person belongs to
{
	Region *reg = getRegion(game);
	return reg->getState();
}
	
Region* Person::getRegion(Game &game) //returns the region that this person belongs to
{
	int x = posx/game.AREA_PER_GRID; int y = posy/game.AREA_PER_GRID;
	return game.getRegion(x,y);
}
