#include "Person.h"

#ifndef WORKER_H
#define WORKER_H //header guards

class Game; //forward declaration

class Worker : public Person
{
	private:
	double salary_per_hour; //passive income every hour if working
	int home_x,home_y; //position of home
	int job_x,job_y; //position of job
	int work_start_hr,work_end_hr; //work hours
	bool is_working = 1;
	public:
	Worker(int posx, int posy, int activity_level, int id, double salary_per_hour=0, int home_x=0, int home_y=0, int job_x=0, int job_y=0, int work_start_hr=6, int work_end_hr=17, bool detected=0): Person(posx,posy,activity_level,id,detected), salary_per_hour(salary_per_hour), home_x(home_x), home_y(home_y), job_x(job_x), job_y(job_y), work_start_hr(work_start_hr), work_end_hr(work_end_hr){};
	void moveWork(Game &game) override; //perform random moves during work period based on game states
	void move(Game &game) override; //move pattern based on game state
	void wander(Game &game) override; //workers cannot leave their home region
	void updateHour(Game &game) override; //remember to update is_working state
	bool isWorking() const {return is_working;}
	double getIncome() override; //get income of person, depending on the derived class
};

#endif
