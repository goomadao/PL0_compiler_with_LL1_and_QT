#pragma execution_character_set("utf-8")
#include "pch.h"
#include "AstNode.h"

AstNode::AstNode()
{
}

AstNode::AstNode(int t, int w, Position p, AstNode * fa)
{
	type = t;
	father = fa;
	wz = w;
	pos = p;
}

void AstNode::setInfo(string s)
{
	info = s;
}

int AstNode::getWz()
{
	return wz;
}

int AstNode::getFatherSize()
{
	return father->child.size();
}

AstNode * AstNode::getFather()
{
	return father;
}

string AstNode::getInfo()
{
	return info;
}

int AstNode::getType()
{
	return type;
}

Position AstNode::getPos()
{
	return pos;
}

