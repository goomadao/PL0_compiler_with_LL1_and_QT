
#ifndef LEXER_H
#define LEXER_H

#include<string>
#include<map>
#include<vector>
#include"token.h"
using std::string;
using std::map;
using std::vector;

enum symbol
{
	ERROR, ID, NUM, PLUSSYM, MINUSSYM, TIMESSYM, DIVISIONSYM, EQLSYM, NEQLSYM, LTSYM, LESYM, GTSYM, GESYM, BECOMESSYM, LPARENSYM, RPARENSYM, SEMICOLONSYM,
	PERIODSYM, COMMASYM, ODDSYM, BEGINSYM, ENDSYM, IFSYM, THENSYM, ELSESYM, WHILESYM, WRITESYM, READSYM, DOSYM, CALLSYM, CONSTSYM, VARSYM, PROCSYM, REPEATSYM, UNTILSYM
};

enum state
{
	BLANK, LETTER, DIGIT, COLON, EQL, LS, GT, PLUS, MINUS, TIMES, DIVISION, LPAREN, RPAREN, COMMA, SEMICOLON, PERIOD, NUL
};



class Lexer
{
private:
	string text;
	map<string, Token> table;
	vector<Token>tokens;
	int r, c, n;
	int cur;
public:
	vector<Token>errorTokens;
	int next();
	Lexer(string t);
	void printTokens();
	Token nextToken();
	bool tokensEnd();
};

#endif