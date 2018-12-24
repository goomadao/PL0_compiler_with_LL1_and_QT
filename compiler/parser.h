#pragma execution_character_set("utf-8")
#pragma once
#ifndef _PARSER_H_
#define _PARSER_H_
#include"AstNode.h"
#include"lexer.h"
#include"grammar.h"
#include"ErrorMessage.h"
#include"symbolTable.h"
#include<string>
#include<stack>
#include<queue>
#include<iostream>
using std::string;
using std::stack;
using std::priority_queue;
using std::cout;
using namespace GrammarSymSpace;

struct pCode
{
	string op;
	int a1, a2;
	pCode(string s, int a, int b)
	{
		op = s;
		a1 = a;
		a2 = b;
	}
};

struct parsingProc
{
	stack<int>s;
	Token t;
	vector<int>v;
	parsingProc() :t(ERRORSYM, "error", 0, 0) { ; }
};

class Parser
{
public:
	Parser(string t);

	void printTokens();
	int lexerTypeToGrammarType(Token g);
	bool isTerminal(int s);
	void printParsing();
	void printSyntaxTree(AstNode * node);
	void errorAdd(Position p, string m);
	void printErrorList();

	void semanticAnalysis();
	void checkSymbolTable();
	int getUseType(AstNode* node);

	void genPCode();
	void handleExpression(AstNode* n, sTable* s);
	void handleExpressionPlus(AstNode* n, sTable* s);
	void handleTerm(AstNode* n, sTable* s);
	void handleTermPlus(AstNode* n, sTable* s);
	void handleFactor(AstNode* n, sTable* s);
	void handleCondition(AstNode* n, sTable* s);
	void handleConditionPlus(AstNode* n, sTable* s);
	void handleStatement(AstNode* n, sTable* s);
	void handleAssignStatement(AstNode* n, sTable* s);
	void handleCallStatement(AstNode* n, sTable* s);
	void handleCompoundStatement(AstNode* n, sTable* s);
	void handleConditionStatement(AstNode* n, sTable* s);
	void handleConditionStatementPlus(AstNode* n, sTable* s);
	void handleLoopStatement(AstNode* n, sTable* s);
	void handleReadStatement(AstNode* n, sTable* s);
	void handleReadVarTable(AstNode* n, sTable* s);
	void handleReadVarTablePlus(AstNode* n, sTable* s);
	void handleReadVar(AstNode* n, sTable* s);
	void handleWriteStatement(AstNode* n, sTable* s);
	void handleWriteExpressionTable(AstNode* n, sTable* s);
	void handleWriteExpressionTablePlus(AstNode* n, sTable* s);
	void handleRepeatStatement(AstNode* n, sTable* s);
	void handleStatementTable(AstNode* n, sTable* s);
	void handleStatementTablePlus(AstNode* n, sTable* s);
	void printPCode();

	list<list<vector<int> > >::iterator getGrammarList();
	list<list<vector<int> > >::iterator getGrammarListEnd();

	map<int, set<int> >::iterator getFirstBegin();
	map<int, set<int> >::iterator getFirstEnd();
	map<int, set<int> >::iterator getFollowBegin();
	map<int, set<int> >::iterator getFollowEnd();
	map<parsingNode, vector<int> >::iterator getParsingTableBegin();
	map<parsingNode, vector<int> >::iterator getParsingTableEnd();
	vector<pCode>::iterator getPCodeBegin();
	vector<pCode>::iterator getPCodeEnd();

	bool errorEmpty();
	




	priority_queue<errorMessage, vector<errorMessage>, greater<errorMessage> >errorList;
	vector<pCode> pcode;
	vector<parsingProc>proc;
private:
	AstNode* syntaxTree;
	//AstNode* currentNode;
	Lexer lexer;
	Grammar grammar;
	stack<int>parsing;//LL1语法分析栈
	Token currentToekn;//当前分析到的token，即输入中剩余代码中的第一个token
	symbolTable* symTable;
	sTable* runTable;
	int startJump;
};


#endif // !_PARSER_H_
