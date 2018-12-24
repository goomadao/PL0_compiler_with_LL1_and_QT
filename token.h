#pragma execution_character_set("utf-8")
#include<string>
#include"position.h"
using namespace std;

#ifndef TOKEN_H
#define TOKEN_H

class Token
{
private:
    int type;
	string val;
	Position pos;
public:
	Token(int t, string s, int r, int c);
	int getType();
	string getVal();
	void printPos();
	Position getPos();
};



#endif