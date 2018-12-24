#pragma execution_character_set("utf-8")
#pragma once
#ifndef _ASTNODE_H_
#define _ASTNODE_H_
#include<string>
#include<vector>
#include"position.h"
using std::string;
using std::vector;



class AstNode
{
public:
	AstNode();
	AstNode(int t, int w, Position p, AstNode* fa=nullptr);
	void setInfo(string s);
	int getWz();
	int getFatherSize();
	AstNode* getFather();
	string getInfo();
	int getType();
	Position getPos();



	vector<AstNode*>child;
private:
	int type;
	string info;
	AstNode* father;
	
	int wz;
	Position pos;
};

#endif // !_ASTNODE_H_
