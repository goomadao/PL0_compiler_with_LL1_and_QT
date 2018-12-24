#pragma execution_character_set("utf-8")
#include"token.h"
#include<string>
using namespace std;


Token::Token(int t, string s, int r, int c)
{
	type = t;
	val = s;
	pos = Position(r, c);
}

int Token::getType()
{
	return type;
}

string Token::getVal()
{
	return val;
}

void Token::printPos()
{
	pos.printPos();
}

Position Token::getPos()
{
	return pos;
}
