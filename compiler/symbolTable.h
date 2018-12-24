#pragma execution_character_set("utf-8")
#pragma once
#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_
#include<set>
#include<string>
#include<iostream>
#include<map>
#include<vector>

#define inf 0x3f3f3f;

using std::set;
using std::string;
using std::cout;
using std::ostream;
using std::map;
using std::vector;
using std::pair;

struct cNode
{
	string name;
	int val;
	cNode(string s)
	{
		name = s;
		val = inf;
	}
	cNode(string s, int v)
	{
		name = s;
		val = v;
	}
	bool operator < (const cNode & a) const
	{
		return name < a.name;
	}
};

struct pos
{
	int pre, off;
	pos(int p, int o)
	{
		pre = p;
		off = o;
	}
};

class symbolTable
{
private:
	symbolTable* pre;
	set<string>v;
	set<string>c;
	set<string>p;
	set<cNode>cc;
public:
	symbolTable();
	symbolTable(symbolTable * pres);
	symbolTable* getPre();
	bool findVar(string s);
	bool findConst(string s);
	bool findProcedure(string s);
	bool findWithNoRecursive(string s);
	void addConst(string s);
	void addVar(string s);
	void addProcedure(string s);
	void addCConst(string s);
	void addCConst(string s, int v);
	void printTable();
};

class sTable
{
private:
	sTable* pre;
	vector<string>v;
	map<string, int>c;
	map<string, int>p;
public:
	sTable();
	sTable(sTable* pres);
	sTable* getPre();
	pos findVar(string s);
	int findConst(string s);
	pos findProcedure(string s);
	bool judgeVarOrConst(string s);
	void addConst(string s, int v);
	void addVar(string s);
	void addProcedure(string s);
	void setProcedure(string s, int v);
	int getVarSize();
};



#endif // !_SYMBOLTABLE_H_
