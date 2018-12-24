#pragma execution_character_set("utf-8")
#ifndef POSITION_H
#define	POSITION_H
#include<iostream>
using std::ostream;

class Position
{
private:
	int row, col;
public:
	Position();
	Position(int r, int c);
	void printPos();
	bool operator < (const Position & p) const;
	bool operator > (const Position &p) const;
	friend ostream & operator <<(ostream& out, const Position & p);
	int getRow();
	int getCol();
};

#endif // !1
