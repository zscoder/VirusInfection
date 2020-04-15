#include "Game.h"
#include "State.h"
#include "Region.h"
#include "Wanderer.h"
#include "Merchant.h"
#include "Worker.h"
#include "MainMenu.h"

using namespace std;

void main2()
{
	MainMenu::maximizeWindow();
	MainMenu menu;
	menu.displayMainMenu();
	/*
	clock_t st = clock();
	Sleep(400);
	clock_t ed = clock();
	double time_elapsed = double(ed-st)/double(CLOCKS_PER_SEC);
	cout<<fixed<<setprecision(2)<<time_elapsed<<'\n';
	IOHandler::askChoices({"I hate this", "I love this", "I don't know"});
	cout<<(Time(3,4)<Time(3,5))<<' '<<(Time(3,0)<Time(2,23))<<' '<<(Time(3,0)<=Time(3,0))<<'\n';
	Wanderer wanderer(1,3,5,0);
	Merchant merchant(5,100,300,20,0.5);
	cout<<merchant.getIncome()<<'\n';
	Worker worker(37,19,28,13,0.375,1,3,67,91);
	cout<<worker.getIncome()<<'\n';
	Game game;
	wanderer.moveShortestPath(4,1,game);
	cout<<wanderer.getCurX()<<' '<<wanderer.getCurY()<<' '<<wanderer.getVisited()<<'\n';
	wanderer.moveShortestPath(4,1,game);
	cout<<wanderer.getCurX()<<' '<<wanderer.getCurY()<<' '<<wanderer.getVisited()<<'\n';
	*/
	/*
	Virus corona(5,3,8,2.7,10);
	Health test;
	test.infect(corona);
	for(int t=0;!test.isHealthy();t++)
	{
		cout<<"STATE AT TIME "<<t<<'\n';
		cout<<test.getSickHours()<<' '<<test.getLethalHours()<<' '<<test.getCureHours()<<'\n';
		test.update();
		if(test.isDead()) {cout<<"PERSON DEAD\n"; return ;}
		if(t>=100) test.cure();
	}
	*/
	/*
	int cc=0;
	while(1)
	{
		cout<<"Iteration "<<++cc<<endl;
		Sleep(400);
		if(kbhit())
		{
			IOHandler io;
			string s = io.getInput("Please enter your command:");
			io.coutc(s,IOHandler::getColorAt(Random::getRand(0,15)));
			cout<<endl;
		}
	}
	*/
}

int main()
{
	main2();
	system("pause");
}
