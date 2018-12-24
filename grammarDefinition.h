#pragma execution_character_set("utf-8")

#pragma once
#ifndef _GRAMMARDEFINITION_H_
#define _GRAMMARDEFINITION_H_
#include<vector>
#include<iostream>
#include<string>
#include"pch.h"
using namespace std;

namespace GrammarSymSpace
{
	enum GrammarSym
	{
		PROCEDURES,
		SUBPROCEDURE,
		DEFINITIONPART,
		CONSTDEFINITIONPART,
		CONSTDEFINITIONTABLE,
		CONSTDEFINITION,
		VARDEFINITIONPART,
		VARDEFINITIONTABLE,
		VARDEFINITION,
		PROCEDURELIST,
		PROCEDUREHEADER,
		PROCEDUREBODY,
		STATEMENT,
		ASSIGHNSTATEMENT,
		CALLSTATEMENT,
		COMPOUNDSTATEMENT,
		CONTIDITIONSTATEMENT,
		LOOPSTATEMENT,
		READSTATEMENT,
		WRITESTATEMENT,
		REPEATSTATEMENT,
		STATEMENTTABLE,
		READVARTABLE,
		READVAR,
		WRITEEXPRESSIONTABLE,
		CONDITION,
		EXPRESSION,
		TERM,
		FACTOR,
		PROCEDURESPLUS,
		SUBPROCEDUREPLUS,
		DEFINITIONPARTPLUS,
		CONSTDEFINITIONPARTPLUS,
		CONSTDEFINITIONTABLEPLUS,
		CONSTDEFINITIONPLUS,
		VARDEFINITIONPARTPLUS,
		VARDEFINITIONTABLEPLUS,
		VARDEFINITIONPLUS,
		PROCEDURELISTPLUS,
		PROCEDUREHEADERPLUS,
		PROCEDUREBODYPLUS,
		STATEMENTPLUS,
		ASSIGHNSTATEMENTPLUS,
		CALLSTATEMENTPLUS,
		COMPOUNDSTATEMENTPLUS,
		CONTIDITIONSTATEMENTPLUS,
		LOOPSTATEMENTPLUS,
		READSTATEMENTPLUS,
		WRITESTATEMENTPLUS,
		REPEATSTATEMENTPLUS,
		STATEMENTTABLEPLUS,
		READVARTABLEPLUS,
		READVARPLUS,
		WRITEEXPRESSIONTABLEPLUS,
		CONDITIONPLUS,
		EXPRESSIONPLUS,
		TERMPLUS,
		FACTORPLUS,
		EXPRESSIONPLUSPLUS,
		CONST,
		ID,
		INTEGER,
		VAR,
		PROCEDURE,
		CALL,
		BEGIN,
		END,
		EQUAL,
		PERIOD,
		SEMICOLON,
		COMMA,
		BECOMES,
		IF,
		THEN,
		ELSE,
		WHILE,
		DO,
		READ,
		LEFTPARENT,
		RIGHTPARENT,
		WRITE,
		REPEAT,
		UNTIL,
		ODD,
		NOTEQUAL,
		LESSTHAN,
		LESSEQUAL,
		GREATERTHAN,
		GREATEREQUAL,
		PLUS,
		MINUS,
		TIMES,
		DIVISION,
		EMPTY,
		ENDEOF,
	};
}


class GrammarDefinition
{
private:
	int leftpart;
	vector<int> rightpart;
	//static string GrammarSymTypes[87];
public:
	static string GrammarSymTypes[95];
	//GrammarDefinition();
	//GrammarDefinition(int left);
	GrammarDefinition(int left, vector<int>right);
	int leftPart() { return leftpart; }
	int rightPartSize() { return rightpart.size(); }
	int rightPartAt(int i) { return rightpart[i]; }
	friend ostream& operator << (ostream& out, const GrammarDefinition & g);
	bool operator == (const GrammarDefinition & g);
	bool startWithNonterminal();
	bool theFirstNonterminalOfRightExpressionEqualToAnotherLeftPart(GrammarDefinition g);
	bool isLeftRecursive();
	bool isLeftPartEqual(GrammarDefinition g);
	GrammarDefinition changeTheFirstNonterminalOfRightExpressionToItsProduction();
	GrammarDefinition changeTheFirstNonterminalOfRightExpressionToItsProduction(GrammarDefinition g);
	GrammarDefinition generateSymWhenEliminate(GrammarDefinition g);
	GrammarDefinition generateSymWhenEliminatePlus();
	GrammarDefinition generateSymWhenEliminatePlusEmpty();
};






#endif // !GRAMMAR_H
