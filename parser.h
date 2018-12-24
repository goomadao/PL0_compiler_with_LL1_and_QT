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
	

private:
	AstNode* syntaxTree;
	//AstNode* currentNode;
	Lexer lexer;
	Grammar grammar;
	stack<int>parsing;//LL1语法分析栈
	Token currentToekn;//当前分析到的token，即输入中剩余代码中的第一个token
	priority_queue<errorMessage, vector<errorMessage>, greater<errorMessage> >errorList;
	symbolTable* symTable;
	sTable* runTable;
	vector<pCode> pcode;
	int startJump;
};


#endif // !_PARSER_H_
