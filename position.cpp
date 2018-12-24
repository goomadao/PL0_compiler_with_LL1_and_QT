#pragma execution_character_set("utf-8")
#include"position.h"
#include<iostream>
using std::cout;

Position::Position()
{
	row = col = 0;
}

Position::Position(int r, int c)
{
	row = r;
	col = c;
}

void Position::printPos()
{
	cout << "(" << row << "," << col << ")";
}

bool Position::operator<(const Position & p) const
{
	if (this->row < p.row)
		return true;
	if (this->row == p.row)
	{
		if (this->col < p.col)
			return true;
	}
	return false;
}

bool Position::operator>(const Position & p) const
{
	if (this->row > p.row)
		return true;
	if (this->row == p.row)
	{
		if (this->col > p.col)
			return true;
	}
	return false;
}

int Position::getRow()
{
	return row;
}

int Position::getCol()
{
	return col;
}

ostream & operator<<(ostream & out, const Position & p)
{
	out << "(" << p.row << "," << p.col << ")";
	return out;
	// TODO: 在此处插入 return 语句
}
