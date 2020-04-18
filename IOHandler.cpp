#include "IOHandler.h"
#include <climits>

string IOHandler::getInput(string message)
{
	if(!message.empty()) cout<<message<<'\n';
	string s; getline(cin,s);
	return s;
}

void IOHandler::coutc(string output, colors color)
{
   HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(handle, color);
   cout<<output;
   SetConsoleTextAttribute(handle, WHITE);
}

int IOHandler::toInt(const string &s) //returns -INF if fail to convert
{
	int ans=0;
	int sign=1;
	int st=0;
	if(!s.empty()&&s[0]=='-') {st=1; sign=-1;}
	for(int i=st;i<int(s.length());i++)
	{
		if(!(s[i]>='0'&&s[i]<='9')) return -INF;
		if(ans*1LL*10>=INT_MAX) return -INF;
		ans*=10; ans+=(s[i]-'0');
	}
	ans*=sign;
	return ans;
}

void IOHandler::coutslow(string s, int DELAY)
{
	for(int i=0;i<int(s.length());i++)
	{
		cout<<s[i];
		Sleep(DELAY);
	}
}

string IOHandler::toString(int x)
{
	string s;
	if(x<0) s+="-";
	x=abs(x);
	if(x==0) s+="0";
	else
	{
		vector<int> digits;
		while(x){digits.push_back(x%10); x/=10;}
		reverse(digits.begin(),digits.end());
		for(int d:digits) s+=char('0'+d);
	}
	return s;
}

int IOHandler::getInt(int l, int r)
{
	string s = getInput();
	int tmp = toInt(s);
	if(!(tmp>=l&&tmp<=r))
	{
		coutc("Error: Invalid input. Input should be an integer in the range ["+toString(l)+", "+toString(r)+"]. Please input your choice again.\n", LIGHTRED);
		return getInt(l,r);
	}
	else
	{
		return tmp;
	}
}

int IOHandler::askChoices(const vector<string> &choices)
{
	for(int i=0;i<int(choices.size());i++)
	{
		cout<<setw(2)<<i+1<<". "<<choices[i]<<'\n';
	}
	cout<<'\n';
	coutc("Enter your choice by inputting the corresponding number:\n", LIGHTCYAN);
	int choice = getInt(1,int(choices.size()));
	return choice-1;
}

vector<string> IOHandler::toWordList(const string &s)
{
	string cur="";
	vector<string> result;
	for(int i=0;i<int(s.length());i++)
	{
		if(s[i]==' ')
		{
			if(!cur.empty()) result.push_back(cur);
			cur="";
		}
		else
		{
			if(s[i]>='A'&&s[i]<='Z') cur+=s[i]-'A'+'a'; //change to lowercase
			else cur+=s[i];
		}
	}
	if(!cur.empty()) result.push_back(cur);
	return result;
}

void IOHandler::errorRange(int l, int r)
{
	coutc("Error: Integer should be in the range ["+toString(l)+", "+toString(r)+"].\n", LIGHTRED);
}
