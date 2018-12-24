#pragma execution_character_set("utf-8")
#include "pch.h"
#include "ErrorMessage.h"

errorMessage::errorMessage(Position p, string m)
{
	pos = p;
	msg = m;
}

bool errorMessage::operator<(const errorMessage & e) const
{
	return this->pos < e.pos;
}

bool errorMessage::operator>(const errorMessage & e) const
{
	return this->pos > e.pos;
}

int errorMessage::getRow()
{
	return pos.getRow();
}

int errorMessage::getColumn()
{
	return pos.getCol();
}

string errorMessage::getMessage()
{
	return msg;
}

ostream & operator<<(ostream & out, const errorMessage & e)
{
	out << e.pos << ":     \t" << e.msg << "\n";
	return out;
	// TODO: 在此处插入 return 语句
}
