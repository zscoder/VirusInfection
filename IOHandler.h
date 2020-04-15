#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <random>
#include <conio.h>
#include <utility>
#include <algorithm>
#include "windows.h"

using namespace std;
 
#ifndef IO_HANDLER_H
#define IO_HANDLER_H //handles i/o for our program
class IOHandler
{
	private:
	static const int INF = int(1e9);
	public:
	enum colors{BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHTGREY,DARKGREY,LIGHTBLUE,LIGHTGREEN,LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,YELLOW,WHITE};
	static colors getColorAt(int v){return static_cast<colors>(v);}
	static string getInput(string message=""); //ask for string input from the user, message = prompt message
	static void coutc(string output, colors color = WHITE); //display output with color, defaults to black
	static int toInt(const string &s); //converts string message to int
	static string toString(int x); //converts int to string
	static vector<string> toWordList(const string &s); //converts user input into a list of words
	static int askChoices(const vector<string> &choices); //ask options
	static int getInt(int l, int r); //get integer input in range [l,r]
	static void errorRange(int l, int r); //print error message for integer not in range [l,r]
};
#endif
