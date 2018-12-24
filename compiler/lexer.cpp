#pragma execution_character_set("utf-8")
#include"pch.h"
#include"lexer.h"
#include<algorithm>
#include<iostream>
#include<bitset>
using std::string;

string Types[35]{ "ERROR", "ID", "NUM", "PLUSSYM", "MINUSSYM", "TIMESSYM", "DIVISIONSYM", "EQLSYM", "NEQLSYM", "LTSYM", "LESYM", "GTSYM", "GESYM", "BECOMESSYM",
"LPARENSYM", "RPARENSYM", "SEMICOLONSYM","PERIODSYM", "COMMASYM","ODDSYM", "BEGINSYM","ENDSYM", "IFSYM", "THENSYM","ELSESYM", "WHILESYM", "WRITESYM", "READSYM", "DOSYM",
"CALLSYM", "CONSTSYM", "VARSYM", "PROCSYM", "REPEATSYM","UNTILSYM" };

int Lexer::next()
{
	if (n >= text.size())
	{
		++n;
		++c;
		return EOF;
	}
	if (text[n] == ' ')
	{
		++n;
		++c;
		return state::BLANK;
	}
	else if (text[n] == '\t')
	{
		++n;
		c += 4;
		return BLANK;
	}
	else if (text[n] == '\n')
	{
		++n;
		c = 1;
		++r;
		return BLANK;
	}
	else if (text[n] <= '9'&&text[n] >= '0')
	{
		++n;
		++c;
		return state::DIGIT;
	}
	else if ((text[n] >= 'a'&&text[n] <= 'z') || (text[n] >= 'A'&&text[n] <= 'Z'))
	{
		++n;
		++c;
		return state::LETTER;
	}
	else if (text[n] == ':')
	{
		++n;
		++c;
		return state::COLON;
	}
	else if (text[n] == '<')
	{
		++n;
		++c;
		return state::LS;
	}
	else if (text[n] == '=')
	{
		++n;
		++c;
		return state::EQL;
	}
	else if (text[n] == '>')
	{
		++n;
		++c;
		return state::GT;
	}
	else if (text[n] == '+')
	{
		++n;
		++c;
		return state::PLUS;
	}
	else if (text[n] == '-')
	{
		++n;
		++c;
		return state::MINUS;
	}
	else if (text[n] == '*')
	{
		++n;
		++c;
		return state::TIMES;
	}
	else if (text[n] == '/')
	{
		++n;
		++c;
		return state::DIVISION;
	}
	else if (text[n] == '(')
	{
		++n;
		++c;
		return state::LPAREN;
	}
	else if (text[n] == ')')
	{
		++n;
		++c;
		return state::RPAREN;
	}
	else if (text[n] == ',')
	{
		++n;
		++c;
		return state::COMMA;
	}
	else if (text[n] == ';')
	{
		++n;
		++c;
		return state::SEMICOLON;
	}
	else if (text[n] == '.')
	{
		++n;
		++c;
		return state::PERIOD;
	}
	else
	{
		++n;
		++c;
		return NUL;
	}
}

Lexer::Lexer(string t)
{
	text = t;
	r = 1;
	c = 1;
	n = 0;
	cur = 0;
	int type = next();
	int leng = text.length();
	int rr, rc, rn;
	while (n < leng + 1)
	{
		/*if (tokens.size())
			cout << tokens[tokens.size() - 1].getVal() << " " << tokens[tokens.size() - 1].getVal().length() << "\n";*/
		switch (type)
		{
		case EOF:
		case NUL:
			rr = r; rc = c - 1; rn = n - 1;
			type = next();
			/*table.insert(pair<string, Token>(text.substr(rn, n - rn - 1), Token(ERRORSYM, text.substr(rn, n - rn - 1), rr, rc)));
			tokens.push_back(Token(ERRORSYM, text.substr(rn, n - rn - 1), rr, rc));*/
			errorTokens.push_back(Token(ERRORSYM, text.substr(rn, n - rn - 1), rr, rc));
			break;
		case BLANK:
			type = next();
			//cout << "空格\n";
			break;
		case DIGIT:
			rr = r; rc = c - 1; rn = n - 1;
			type = next();
			while (type == DIGIT)
			{
				type = next();
			}
			if (type == PERIOD)
			{
				type = next();
				if (type != NUM)
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn - 2), Token(NUM, text.substr(rn, n - rn - 2), rr, rc)));
					tokens.push_back(Token(NUM, text.substr(rn, n - rn - 2), rr, rc));
					table.insert(pair<string, Token>(".", Token(PERIOD, ".", r, c)));
					tokens.push_back(Token(PERIOD, ".", r, c));
					break;
				}
				else
				{
					while (type == NUM)
					{
						type = next();
					}
				}
			}
			table.insert(pair<string, Token>(text.substr(rn, n - rn - 1), Token(NUM, text.substr(rn, n - rn - 1), rr, rc)));
			tokens.push_back(Token(NUM, text.substr(rn, n - rn - 1), rr, rc));
			break;
		case LETTER:
			rr = r; rc = c - 1; rn = n - 1;
			type = next();
			while (type == DIGIT || type == LETTER)
			{
				type = next();
			}
			--n;
			--n;//和下面的++n抵消
			//cout << text.substr(rn, n - rn + 1) << "\n";
			{
				string revalue = text.substr(rn, n - rn + 1);
				if (revalue == "const")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(CONSTSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(CONSTSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "var")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(VARSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(VARSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "procedure")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(PROCSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(PROCSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "begin")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(BEGINSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(BEGINSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "end")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(ENDSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(ENDSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "odd")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(ODDSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(ODDSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "if")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(IFSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(IFSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "then")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(THENSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(THENSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "else")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(ELSESYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(ELSESYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "call")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(CALLSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(CALLSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "while")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(WHILESYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(WHILESYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "do")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(DOSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(DOSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "read")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(READSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(READSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "write")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(WRITESYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(WRITESYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "repeat")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(REPEATSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(REPEATSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else if (revalue == "until")
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(UNTILSYM, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(UNTILSYM, text.substr(rn, n - rn + 1), rr, rc));
				}
				else
				{
					table.insert(pair<string, Token>(text.substr(rn, n - rn + 1), Token(ID, text.substr(rn, n - rn + 1), rr, rc)));
					tokens.push_back(Token(ID, text.substr(rn, n - rn + 1), rr, rc));
				}
			}
			++n;
			++n;
			break;
		case COLON:
			type = next();
			if (type == state::EQL)
			{
				table.insert(pair<string, Token>(":=", Token(BECOMESSYM, ":=", r, c - 2)));
				tokens.push_back(Token(BECOMESSYM, ":=", r, c - 2));
				type = next();
			}
			else
			{
				/*table.insert(pair<string, Token>(":", Token(ERRORSYM, ":", r, c - 2)));
				tokens.push_back(Token(ERRORSYM, ":", r, c - 2));*/
				errorTokens.push_back(Token(ERRORSYM, ":", r, c - 2));
			}
			break;
		case LS:
			type = next();
			if (type == state::EQL)
			{
				table.insert(pair<string, Token>("<=", Token(LESYM, "<=", r, c - 2)));
				tokens.push_back(Token(LESYM, "<=", r, c - 2));
				type = next();
			}
			else if (type == state::GT)
			{
				table.insert(pair<string, Token>("<>", Token(NEQLSYM, "<>", r, c - 2)));
				tokens.push_back(Token(NEQLSYM, "<>", r, c - 2));
				type = next();
			}
			else
			{
				table.insert(pair<string, Token>("<", Token(LTSYM, "<", r, c - 2)));
				tokens.push_back(Token(LTSYM, "<", r, c - 2));
			}
			break;
		case state::GT:
			type = next();
			if (type == state::EQL)
			{
				table.insert(pair<string, Token>(">=", Token(GESYM, ">=", r, c - 2)));
				tokens.push_back(Token(GESYM, ">=", r, c - 2));
				type = next();
			}
			else
			{
				table.insert(pair<string, Token>(">", Token(symbol::GTSYM, ">", r, c - 2)));
				tokens.push_back(Token(symbol::GTSYM, ">", r, c - 2));
			}
			break;
		case EQL:
			table.insert(pair<string, Token>("=", Token(EQLSYM, "=", r, c - 1)));
			tokens.push_back(Token(EQLSYM, "=", r, c - 1));
			type = next();
			break;
		case state::PLUS:
			table.insert(pair<string, Token>("+", Token(symbol::PLUSSYM, "+", r, c - 1)));
			tokens.push_back(Token(symbol::PLUSSYM, "+", r, c - 1));
			type = next();
			break;
		case state::MINUS:
			table.insert(pair<string, Token>("-", Token(symbol::MINUSSYM, "-", r, c - 1)));
			tokens.push_back(Token(symbol::MINUSSYM, "-", r, c - 1));
			type = next();
			break;
		case state::TIMES:
			table.insert(pair<string, Token>("*", Token(symbol::TIMESSYM, "*", r, c - 1)));
			tokens.push_back(Token(symbol::TIMESSYM, "*", r, c - 1));
			type = next();
			break;
		case state::DIVISION:
			table.insert(pair<string, Token>("/", Token(symbol::DIVISIONSYM, "/", r, c - 1)));
			tokens.push_back(Token(symbol::DIVISIONSYM, "/", r, c - 1));
			type = next();
			break;
		case state::LPAREN:
			table.insert(pair<string, Token>("(", Token(symbol::LPARENSYM, "(", r, c - 1)));
			tokens.push_back(Token(symbol::LPARENSYM, "(", r, c - 1));
			type = next();
			break;
		case state::RPAREN:
			table.insert(pair<string, Token>(")", Token(symbol::RPARENSYM, ")", r, c - 1)));
			tokens.push_back(Token(symbol::RPARENSYM, ")", r, c - 1));
			type = next();
			break;
		case state::COMMA:
			table.insert(pair<string, Token>(",", Token(symbol::COMMASYM, ",", r, c - 1)));
			tokens.push_back(Token(symbol::COMMASYM, ",", r, c - 1));
			type = next();
			break;
		case state::SEMICOLON:
			table.insert(pair<string, Token>(";", Token(symbol::SEMICOLONSYM, ";", r, c - 1)));
			tokens.push_back(Token(symbol::SEMICOLONSYM, ";", r, c - 1));
			type = next();
			break;
		case state::PERIOD:
			table.insert(pair<string, Token>(".", Token(symbol::PERIODSYM, ".", r, c - 1)));
			tokens.push_back(Token(symbol::PERIODSYM, ".", r, c - 1));
			type = next();
			break;
		default:

			break;
		}
	}
}


void Lexer::printTokens()
{
	for (int i = 0; i < tokens.size(); ++i)
	{
		cout << Types[tokens[i].getType()];
		for (int j = 0; j < 15 - Types[tokens[i].getType()].size(); ++j)
		{
			cout << " ";
		}
		cout << tokens[i].getVal();
		for (int j = 0; j < 15 - tokens[i].getVal().size(); ++j)
		{
			cout << " ";
		}
		cout << tokens[i].getVal().size() << " ";
		tokens[i].printPos();
		cout << "\n";
	}
}

Token Lexer::nextToken()
{
	if (cur != tokens.size())
		return tokens[cur++];
	if (tokens.empty())
		return Token(PERIODSYM, ".", 1, 1);
	else
		return Token(PERIODSYM, ".", tokens[tokens.size() - 1].getPos().getRow(), tokens[tokens.size() - 1].getPos().getCol());
}

bool Lexer::tokensEnd()
{
	return tokens.size() == cur;
}
