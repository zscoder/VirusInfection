#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <map>
#include <ctime>
#include <random>
#include <conio.h>
#include <utility>
#include <algorithm>
#if       _WIN32_WINNT < 0x0500
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT   0x0500
#endif
#include "windows.h"

using namespace std;
 
#ifndef RANDOM_H
#define RANDOM_H //random number generator
class Random
{
	private:
	static mt19937 rng;
	public:
	static int getRand(int l, int r) {return uniform_int_distribution<int>(l,r)(rng);}
	static bool getDecision(double p) {return (uniform_real_distribution<double>(0.0,1.0)(rng) <= p);} //return 1 with probability p
	static double getRand(double l, double r) {return uniform_real_distribution<double>(l,r)(rng);}
};
#endif 
