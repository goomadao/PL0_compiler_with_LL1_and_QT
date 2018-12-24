#pragma execution_character_set("utf-8")

#pragma once
#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_
#include<vector>
#include<list>
#include<set>
#include<map>
#include"grammarDefinition.h"
using std::vector;
using std::list;
using std::set;
using std::map;
using namespace GrammarSymSpace;

struct parsingNode
{
	int nonTerminal, terminal;
	parsingNode(int i, int j)
	{
		nonTerminal = i;
		terminal = j;
	}
	bool operator < (const parsingNode & other) const
	{
		if (nonTerminal == other.nonTerminal)
			return terminal < other.terminal;
		else
			return nonTerminal < other.nonTerminal;
	}
};

class Grammar
{
public:
	Grammar();
	void getOriginGrammar();//未使用
	void getOriginList();//获取原始的PL/0文法并且存储在list中
	void printGrammarList();
	//void transportToList();
	void eliminateLeftRecursive();//消除左递归
	void leftFactoring();//提取左公因式

	//计算First集
	void getFirst();
	void calculateFirst(int x);
	void printFirst();

	//计算Follow集
	void getFollow();
	void calculateFollow();
	int getFollowSize();
	void printFollow();

	//构造预测分析表
	void getParsingTable();
	void printParsingTable();

	map<parsingNode, vector<int> >parsingTable;//预测分析表
	map<int, set<int> >follow;
	map<int, set<int> >first;

	list<list<vector<int> > >::iterator getGrammarList();
	list<list<vector<int> > >::iterator getGrammarListEnd();
private:
	vector<GrammarDefinition> grammarSet;//未使用


	/*
	第一个vector：文法产生式左端
	第二个vector：第一个右部产生式
	第三个vector：第二个右部产生式
	……
	*/
	list< list< vector<int> > > grammarList;//文法list
	
	
	
};


#endif // !GRAMMAR_H
