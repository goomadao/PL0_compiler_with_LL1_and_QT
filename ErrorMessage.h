#pragma execution_character_set("utf-8")
#pragma once
#ifndef _ERRORMESSAGE_H_
#define _ERRORMESSAGE_H_
#include"position.h"
#include<string>
#include<iostream>
using std::string;
using std::ostream;

class errorMessage
{
private:
	Position pos;
	string msg;
public:
	errorMessage(Position p, string m);
	bool operator < (const errorMessage & e) const;
	bool operator > (const errorMessage & e) const;
	friend ostream & operator <<(ostream & out, const errorMessage & e);
};


#endif // !_ERRORMESSAGE_H_
