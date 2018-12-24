#pragma execution_character_set("utf-8")
/*Todo:
	i>>10的报错没有解决

*/


#include "pch.h"
#include "parser.h"

Parser::Parser(string t) :lexer(t), currentToekn(lexer.nextToken())
{
	for (int i = 0; i < lexer.errorTokens.size(); ++i)
	{
		if (lexer.errorTokens[i].getVal() == ":")
			errorAdd(lexer.errorTokens[i].getPos(), "\":\"must be followed by \"=\"");
		else
			errorAdd(lexer.errorTokens[i].getPos(), "Unknown token \"" + lexer.errorTokens[i].getVal() + "\"");
	}
	parsing.push(PROCEDURES);
	syntaxTree = new AstNode(PROCEDURES, 0, currentToekn.getPos());
	symTable = new symbolTable();
	runTable = new sTable();
	AstNode* currentNode = syntaxTree;
	while (!parsing.empty())
	{
		cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
		if (parsing.top() == GrammarSymSpace::PERIOD&&currentToekn.getType() == symbol::PERIODSYM)
		{
			cout << "语法分析完成\n";
			break;
		}
		if (parsing.top() == GrammarSymSpace::EMPTY)//栈顶为空，直接出栈，并且向上寻找下一个该分析的节点（向父节点递归）
		{
			currentNode->setInfo("ε");
			parsing.pop();
			while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
			{
				currentNode = currentNode->getFather();
				if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
				{
					cout << "无法找到下一个待分析的节点\n";
					if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
						break;
					else
						return;
				}
			}
			if (currentNode != syntaxTree)
				currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

			cout << "\n";
			continue;
		}
		if (isTerminal(parsing.top()))//栈顶是终结符，和剩余代码比对，匹配则都出栈，否则进入错误处理
		{
			if (parsing.top() == lexerTypeToGrammarType(currentToekn))//终结符匹配
			{
				currentNode->setInfo(currentToekn.getVal());//设置这个节点的属性=token的Val
				while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
				{
					currentNode = currentNode->getFather();
					if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
					{
						cout << "无法找到下一个待分析的节点\n";
						if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
							break;
						else
							return;
					}
				}
				if (currentNode != syntaxTree)
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "和" << currentToekn.getVal() << "匹配,出栈\n";
				parsing.pop();
				if (lexer.tokensEnd() && parsing.empty())
					;
				else
					currentToekn = lexer.nextToken();
			}
			else//不匹配，开始错误处理
			{
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "和" << currentToekn.getVal() << "不匹配!\n";





				errorAdd(currentToekn.getPos(), "Expect \"" + GrammarDefinition::GrammarSymTypes[parsing.top()] + "\", while found \"" + currentToekn.getVal() + "\"");

				//首先查看自己后面的兄弟节点是否存在分号和end，
				//如果存在，就直接停止寻找follow集
				//如果不存在，就开始向父节点递归，先记录这个节点的follow集，然后也查看这个节点后面的兄弟节点是否存在分号、end或者标志程序结束的小数点，一直往上递归，直到兄弟节点中找到了分号、end或者标志程序结束的小数点就停止递归。
				//然后开始扫描token，
				//如果这个token在之前的记录里面，那么说明应该重新从这个token开始分析，
				//如果不在，则继续扫描下一个token，直到扫描到end、分号或者标志程序结束的小数点，强行中断扫描，并且从这里重新开始分析
				//然后根据这个token确定应该把分析栈中哪些部分出栈（这些部分被跳过），然后重新开始语法分析

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				while (tmp != syntaxTree)//一直往上递归找follow集，直到遇到; end或者.
				{
					cout << "currentNode:  " << GrammarDefinition::GrammarSymTypes[tmp->getType()] << "\n";
					bool flag = 0;
					for (int i = tmp->getWz() + 1; i < tmp->getFather()->child.size(); ++i)
					{
						//停止集，从这里重新开始语法分析
						if (tmp->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::END ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::CONST ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::VAR ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PROCEDURE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::CALL ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::BEGIN ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::IF ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::WHILE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::READ ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::WRITE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::REPEAT)
						{
							flag = 1;
							break;
						}
					}
					if (flag)
						break;





					tmp = tmp->getFather();
					set<int>::iterator it;
					for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
					{
						follows.insert(*it);
					}
				}
				while (1)
				{

					//遇到了const var procedure call begin if while read write repeat这些标志语句开始的token，从这里重新开始语法分析
					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CONST ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::VAR ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PROCEDURE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CALL ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::BEGIN ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::IF ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WHILE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::READ ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WRITE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::REPEAT)
					{




						AstNode* tempCurrentNode = currentNode;
						while (tempCurrentNode->getType() != GrammarSymSpace::CONSTDEFINITIONPART &&
							tempCurrentNode->getType() != GrammarSymSpace::VARDEFINITIONPART &&
							tempCurrentNode->getType() != GrammarSymSpace::PROCEDURELIST &&
							tempCurrentNode->getType() != GrammarSymSpace::STATEMENT)
						{
							tempCurrentNode = tempCurrentNode->getFather();
						}
						int currentType = tempCurrentNode->getType();
						if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CONST &&
							(
								currentType == GrammarSymSpace::VARDEFINITIONPART ||
								currentType == GrammarSymSpace::PROCEDURELIST ||
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}
						else if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::VAR &&
							(
								currentType == GrammarSymSpace::PROCEDURELIST ||
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}
						else if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PROCEDURELIST &&
							(
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}




						while (
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::CONST) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::VAR) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::PROCEDURE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::CALL) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::BEGIN) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::IF) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::WHILE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::READ) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::WRITE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::REPEAT) == grammar.first.find(parsing.top())->second.end() &&
							!(
							(lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CALL ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::BEGIN ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::IF ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WHILE ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::READ ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WRITE ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::REPEAT) &&
								parsing.top() == GrammarSymSpace::STATEMENTTABLEPLUS
								)
							)
						{
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::CONST) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::VAR) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::PROCEDURE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::CALL) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::BEGIN) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::IF) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::WHILE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::READ) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::WRITE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::REPEAT) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::STATEMENTTABLEPLUS)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									if (currentNode->getType() == GrammarSymSpace::STATEMENTTABLEPLUS)
									{
										parsing.push(GrammarSymSpace::STATEMENT);
										vector<AstNode*>temp;
										temp.push_back(new AstNode(GrammarSymSpace::SEMICOLON, 0, currentToekn.getPos(), currentNode));
										temp.push_back(new AstNode(GrammarSymSpace::STATEMENT, 1, currentToekn.getPos(), currentNode));
										temp.push_back(new AstNode(GrammarSymSpace::STATEMENTTABLEPLUS, 0, currentToekn.getPos(), currentNode));
										currentNode->child = temp;
										currentNode = currentNode->child[1];
									}
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
						break;
					}



					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//当前指向的token在follows中
					{
						if (currentNode->getType() == lexerTypeToGrammarType(currentToekn))
						{
							terminalMatch = 1;
							cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
							break;
						}
						parsing.pop();
						while (currentNode != syntaxTree)
						{
							for (int i = currentNode->getWz(); i < currentNode->getFather()->child.size() - 1; ++i)
							{
								parsing.pop();
							}//首先把分析栈中这个节点所在的产生式中还没分析的部分出栈（因为要跳过）
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//一定是非终结符
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
								cout << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "\n\n";
								flag = 1;
								break;
							}
						}
						if (flag)
							break;
					}

					//如果遇到了; end或者.，就应该从这里重新开始语法分析了，其他的都要跳过
					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::SEMICOLON ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::END ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PERIOD)
					{
						while (parsing.top() != GrammarSymSpace::SEMICOLON &&
							parsing.top() != GrammarSymSpace::END &&
							parsing.top() != GrammarSymSpace::PERIOD)//把栈顶到; end或者.之间的符号全部出栈（被跳过）
						{
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (currentNode->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::END ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
						break;
					}






					if (lexer.tokensEnd())
					{
						cout << "代码已空，语法分析失败\n";
						return;
					}
					currentToekn = lexer.nextToken();//读入下一个token
				}
				if (flag)//还需要将这个非终结符出栈，下一个非终结符才能和currentToken匹配
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								break;
							else
								return;
						}
					}
					if (currentNode != syntaxTree)
						currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

				}
				else if (!terminalMatch)//找不到匹配
				{
					cout << "无法跳出该错误，语法分析失败\n";
					return;
				}
				else//直接终结符匹配了，大多数情况下是多了某个符号，删去即可（删去表现为上面跳过了这个token）
				{
					;
				}




			}
		}
		else//栈顶为非终结符，查预测表，有相应的项目则用产生式替换，否则进入错误处理
		{
			//预测表中没有对应的项目，进入错误处理
			if (grammar.parsingTable.find(parsingNode(parsing.top(), lexerTypeToGrammarType(currentToekn)))->second.empty())
			{
				cout << "对于(" << GrammarDefinition::GrammarSymTypes[parsing.top()] << "," << currentToekn.getVal() << "),预测表中没有对应的项目\n";

				errorAdd(currentToekn.getPos(), "Unexpected token \"" + currentToekn.getVal() + "\"");


				//先找出从这个节点向上递归所有父节点的所有follow集，一直找到根节点(这个节点不要找，因为这个节点开始不匹配！)
				//先判断当前指向的token，再取下一个token，直到这个token和之前找到的follow集中的某个终结符匹配
				//currentNode开始向父节点递归，直到这个节点的follow集中包含这个token（这个节点也要检查）

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				//set<int>::iterator it;//先加入当前节点的follow集
				//for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
				//{
				//	follows.insert(*it);
				//}
				while (1)//一直递归到根节点找到所有的follow中的终结符
				{
					cout << "currentNode:  " << GrammarDefinition::GrammarSymTypes[tmp->getType()] << "\n";
					bool flag = 0;
					for (int i = tmp->getWz() + 1; i < tmp->getFather()->child.size(); ++i)
					{
						if (tmp->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::END ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::CONST ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::VAR ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PROCEDURE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::CALL ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::BEGIN ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::IF ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::WHILE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::READ ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::WRITE ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::REPEAT)
						{
							flag = 1;
							break;
						}
					}
					if (flag)
						break;




					tmp = tmp->getFather();
					set<int>::iterator it;
					for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
					{
						follows.insert(*it);
					}
				}
				while (1)
				{

					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CONST ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::VAR ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PROCEDURE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CALL ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::BEGIN ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::IF ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WHILE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::READ ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WRITE ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::REPEAT)
					{



						AstNode* tempCurrentNode = currentNode;
						while (tempCurrentNode->getType() != GrammarSymSpace::CONSTDEFINITIONPART &&
							tempCurrentNode->getType() != GrammarSymSpace::VARDEFINITIONPART &&
							tempCurrentNode->getType() != GrammarSymSpace::PROCEDURELIST &&
							tempCurrentNode->getType() != GrammarSymSpace::STATEMENT)
						{
							tempCurrentNode = tempCurrentNode->getFather();
						}
						int currentType = tempCurrentNode->getType();
						if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CONST &&
							(
								currentType == GrammarSymSpace::VARDEFINITIONPART ||
								currentType == GrammarSymSpace::PROCEDURELIST ||
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}
						else if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::VAR &&
							(
								currentType == GrammarSymSpace::PROCEDURELIST ||
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}
						else if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PROCEDURELIST &&
							(
								currentType == GrammarSymSpace::STATEMENT
								)
							)
						{
							currentToekn = lexer.nextToken();
						}





						/*cout << "还没进入while循环\n";
						if (
							!(
							(lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CALL ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::BEGIN ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::IF ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WHILE ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::READ ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WRITE ||
							lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::REPEAT) &&
							parsing.top() == GrammarSymSpace::STATEMENTTABLEPLUS
							)
							)
						{
							cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "的first集中没有const\n";
						}*/
						while (
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::CONST) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::VAR) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::PROCEDURE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::CALL) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::BEGIN) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::IF) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::WHILE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::READ) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::WRITE) == grammar.first.find(parsing.top())->second.end() &&
							grammar.first.find(parsing.top())->second.find(GrammarSymSpace::REPEAT) == grammar.first.find(parsing.top())->second.end() &&
							!(
							(lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::CALL ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::BEGIN ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::IF ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WHILE ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::READ ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::WRITE ||
								lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::REPEAT) &&
								parsing.top() == GrammarSymSpace::STATEMENTTABLEPLUS
								)
							)
						{
							cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "的first集中找不到停止集，也没有到达语句表+\n";
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::CONST) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::VAR) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::PROCEDURE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::CALL) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::BEGIN) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::IF) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::WHILE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::READ) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::WRITE) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									grammar.first.find(currentNode->getFather()->child[i]->getType())->second.find(GrammarSymSpace::REPEAT) != grammar.first.find(currentNode->getFather()->child[i]->getType())->second.end() ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::STATEMENTTABLEPLUS)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									if (currentNode->getType() == GrammarSymSpace::STATEMENTTABLEPLUS)
									{
										parsing.push(GrammarSymSpace::STATEMENT);
										vector<AstNode*>temp;
										temp.push_back(new AstNode(GrammarSymSpace::SEMICOLON, 0, currentToekn.getPos(), currentNode));
										temp.push_back(new AstNode(GrammarSymSpace::STATEMENT, 1, currentToekn.getPos(), currentNode));
										temp.push_back(new AstNode(GrammarSymSpace::STATEMENTTABLEPLUS, 0, currentToekn.getPos(), currentNode));
										currentNode->child = temp;
										currentNode = currentNode->child[1];
									}
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
						break;
					}


					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//当前指向的token在follows中
					{
						if (currentNode->getType() == lexerTypeToGrammarType(currentToekn))
						{
							terminalMatch = 1;
							cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
							break;
						}
						parsing.pop();
						//if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
						//{
						//	//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
						//	flag = 1;
						//	break;
						//}
						while (currentNode != syntaxTree)
						{
							for (int i = currentNode->getWz(); i < currentNode->getFather()->child.size() - 1; ++i)
							{
								parsing.pop();
							}//首先把分析栈中这个节点所在的产生式中还没分析的部分出栈（因为要跳过）
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//一定是非终结符
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
								flag = 1;
								break;
							}
						}
						if (flag)
							break;
					}



					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::SEMICOLON ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::END ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PERIOD)
					{
						while (parsing.top() != GrammarSymSpace::SEMICOLON &&
							parsing.top() != GrammarSymSpace::END &&
							parsing.top() != GrammarSymSpace::PERIOD)
						{
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (currentNode->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::END ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
						break;
					}





					if (lexer.tokensEnd())
					{
						cout << "代码已空，语法分析失败\n";
						return;
					}
					currentToekn = lexer.nextToken();//读入下一个token
				}
				if (flag)//还需要将这个非终结符出栈，下一个非终结符才能和currentToken匹配
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								break;
							else
								return;
						}
					}
					if (currentNode != syntaxTree)
						currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

				}
				else if (!terminalMatch)//找不到匹配
				{
					cout << "无法跳出该错误，语法分析失败\n";
					return;
				}
				else//直接终结符匹配了，大多数情况下是多了某个符号，删去即可（删去表现为上面跳过了这个token）
				{
					;
				}





			}
			else//预测表中找到了对应的项目，用产生式替换。
			{
				int top = parsing.top();

				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "出栈;";

				parsing.pop();

				vector<int>::iterator i = grammar.parsingTable.find(parsingNode(top, lexerTypeToGrammarType(currentToekn)))->second.end();
				--i;
				stack<int>expand;
				vector<AstNode*>temp;
				for (; i != grammar.parsingTable.find(parsingNode(top, lexerTypeToGrammarType(currentToekn)))->second.begin(); --i)
				{
					//if (*i == EMPTY)
						//continue;
					cout << GrammarDefinition::GrammarSymTypes[*i] << " ";
					parsing.push(*i);
					expand.push(*i);
				}
				//if (*i != EMPTY)
				{
					cout << GrammarDefinition::GrammarSymTypes[*i];
					parsing.push(*i);
					expand.push(*i);
				}
				int count = 0;
				while (!expand.empty())
				{
					temp.push_back(new AstNode(expand.top(), count++, currentToekn.getPos(), currentNode));//count表示在Vector中的索引,父节点是currentNode
					expand.pop();
				}
				currentNode->child = temp;
				if (temp.empty())
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								;
							else
								return;
						}
					}
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个
				}
				else
					currentNode = temp[0];//currentNode已经出栈了，所以孩子节点中的第一个一定会成为currentNode
				cout << "入栈\n";
			}
		}
		cout << "栈:";
		printParsing();
		cout << "\n";
	}
	AstNode* baocun = syntaxTree;
	printSyntaxTree(syntaxTree);//输出语法树的叶节点（可能会输出ε）
	syntaxTree = baocun;
	cout << "树输出成功\n";


	if (errorList.empty())
		semanticAnalysis();//语义分析



	if (errorList.empty())
	{
		genPCode();
		printPCode();
	}


}

void Parser::printTokens()
{
	lexer.printTokens();
}

int Parser::lexerTypeToGrammarType(Token g)
{
	int t = g.getType();
	switch (t)
	{
	case symbol::ID:
		return GrammarSym::ID;
	case symbol::NUM:
		return GrammarSym::INTEGER;
	case symbol::PLUSSYM:
		return GrammarSym::PLUS;
	case symbol::MINUSSYM:
		return GrammarSym::MINUS;
	case symbol::TIMESSYM:
		return GrammarSym::TIMES;
	case symbol::DIVISIONSYM:
		return GrammarSym::DIVISION;
	case symbol::EQLSYM:
		return GrammarSym::EQUAL;
	case symbol::NEQLSYM:
		return GrammarSym::NOTEQUAL;
	case symbol::LTSYM:
		return GrammarSym::LESSTHAN;
	case symbol::LESYM:
		return GrammarSym::LESSEQUAL;
	case symbol::GTSYM:
		return GrammarSym::GREATERTHAN;
	case symbol::GESYM:
		return GrammarSym::GREATEREQUAL;
	case symbol::BECOMESSYM:
		return GrammarSym::BECOMES;
	case symbol::LPARENSYM:
		return GrammarSym::LEFTPARENT;
	case symbol::RPARENSYM:
		return GrammarSym::RIGHTPARENT;
	case symbol::SEMICOLONSYM:
		return GrammarSym::SEMICOLON;
	case symbol::PERIODSYM:
		return GrammarSym::PERIOD;
	case symbol::COMMASYM:
		return GrammarSym::COMMA;
	case symbol::ODDSYM:
		return GrammarSym::ODD;
	case symbol::BEGINSYM:
		return GrammarSym::BEGIN;
	case symbol::ENDSYM:
		return GrammarSym::END;
	case symbol::IFSYM:
		return GrammarSym::IF;
	case symbol::THENSYM:
		return GrammarSym::THEN;
	case symbol::ELSESYM:
		return GrammarSym::ELSE;
	case symbol::WHILESYM:
		return GrammarSym::WHILE;
	case symbol::WRITESYM:
		return GrammarSym::WRITE;
	case symbol::READSYM:
		return GrammarSym::READ;
	case symbol::DOSYM:
		return GrammarSym::DO;
	case symbol::CALLSYM:
		return GrammarSym::CALL;
	case symbol::CONSTSYM:
		return GrammarSym::CONST;
	case symbol::VARSYM:
		return GrammarSym::VAR;
	case symbol::PROCSYM:
		return GrammarSym::PROCEDURE;
	case symbol::REPEATSYM:
		return GrammarSym::REPEAT;
	case symbol::UNTILSYM:
		return GrammarSym::UNTIL;

	default:
		break;
	}
	return 0;
}

bool Parser::isTerminal(int s)
{
	if (s > GrammarSym::EXPRESSIONPLUSPLUS)
		return true;
	return false;
}

void Parser::printParsing()
{
	stack<int>temp = parsing;
	while (!temp.empty())
	{
		cout << GrammarDefinition::GrammarSymTypes[temp.top()] << " ";
		temp.pop();
	}
	cout << "\n";
}

void Parser::printSyntaxTree(AstNode * node)
{
	if (node->child.empty())
	{
		/*if (node->getInfo() == "")
			cout << GrammarDefinition::GrammarSymTypes[node->getType()] << "             aaa\n";
		else
			cout << node->getInfo() << "             bbb\n";*/
		cout << node->getInfo() << "\n";
		return;
	}
	for (int i = 0; i < node->child.size(); ++i)
	{
		printSyntaxTree(node->child[i]);
	}
}

void Parser::errorAdd(Position p, string m)
{
	errorList.push(errorMessage(p, m));
}

void Parser::printErrorList()
{
	if (errorList.empty())
	{
		cout << "No error found.\n";
		return;
	}
	priority_queue<errorMessage>tmp;
	while (!errorList.empty())
	{
		cout << errorList.top();
		tmp.push(errorList.top());
		errorList.pop();
	}
	while (!tmp.empty())
	{
		errorList.push(tmp.top());
		tmp.pop();
	}
}

void Parser::semanticAnalysis()
{
	checkSymbolTable();
}

void Parser::checkSymbolTable()
{
	AstNode* currentNode = syntaxTree;
	symbolTable* currentTable = symTable;
	while (1)
	{
		/*int js = 0;
		symbolTable* tst = currentTable;
		while (tst != nullptr)
		{
			++js;
			tst = tst->getPre();
		}
		cout << "currentNode: " << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "  Level:" << js << "\n";*/




		if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
		{
			currentTable = new symbolTable(currentTable);
		}
		if (currentNode->getType() == GrammarSymSpace::ID)//如果当前节点是ID，向上递归查找是声明还是使用
		{
			int flag = getUseType(currentNode);
			if (flag == -4)
			{
				if (!currentTable->findVar(currentNode->getInfo()) && !currentTable->findConst(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \"" + currentNode->getInfo() + "\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -3)
			{
				if (!currentTable->findProcedure(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined procedure \"" + currentNode->getInfo() + "\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -2)
			{
				if (!currentTable->findVar(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \"" + currentNode->getInfo() + "\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -1)
			{
				if (!currentTable->findConst(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \"" + currentNode->getInfo() + "\"");
				}
			}
			else if (flag == 1)
			{
				if (!currentTable->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->addConst(currentNode->getInfo());
					if (currentNode->getFather()->child.size() == 3)
					{
						bool flag = 1;
						AstNode* tmpNode = currentNode->getFather()->child[2];
						for (int i = 0; i < tmpNode->getInfo().size(); ++i)
						{
							if (!isdigit(tmpNode->getInfo()[i]))
							{
								flag = 0;
								break;
							}
						}
						if (flag)
						{
							currentTable->addCConst(currentNode->getInfo(), stoi(currentNode->getFather()->child[2]->getInfo()));
							cout << "constSymbol:" << currentNode->getInfo() << "   Val:" << stoi(currentNode->getFather()->child[2]->getInfo()) << "\n";
						}
						else
						{
							currentTable->addCConst(currentNode->getInfo());
						}
					}
					//cout << "const声明成功\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\"" + currentNode->getInfo() + "\" has been defined, redefinition is not allowed.");
				}
			}
			else if (flag == 2)
			{
				if (!currentTable->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->addVar(currentNode->getInfo());
					//cout << "var声明成功\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\"" + currentNode->getInfo() + "\" has been defined, redefinition is not allowed.");
				}
			}
			else if (flag == 3)
			{
				if (!currentTable->getPre()->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->getPre()->addProcedure(currentNode->getInfo());
					//cout << "procedure声明成功\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\"" + currentNode->getInfo() + "\" has been defined, redefinition is not allowed.");
				}
			}
			else
			{
				errorAdd(currentNode->getPos(), "Unknown error.");
			}
		}




		//移向下一个节点
		if (!currentNode->child.empty())
		{
			currentNode = currentNode->child[0];
		}
		else
		{
			while (currentNode->getWz() == currentNode->getFather()->child.size() - 1)
			{
				currentNode = currentNode->getFather();
				if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
					currentTable = currentTable->getPre();
				if (currentNode == syntaxTree)
				{
					return;
				}
			}
			currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];
		}
	}

}


/*
-4——到var或者const里面去查找
-3——到procedure查找
-2——到var查找
-1——到const查找
 0——错误
 1——声明const
 2——声明var
 3——声明procedure
*/
int Parser::getUseType(AstNode * node)
{
	if (node->getType() == GrammarSymSpace::CONSTDEFINITION)//声明const
	{
		return 1;
	}
	if (node->getType() == GrammarSymSpace::VARDEFINITION)//声明var
	{
		return 2;
	}
	if (node->getType() == GrammarSymSpace::PROCEDUREHEADER)//声明procedure
	{
		return 3;
	}
	if (node->getType() == GrammarSymSpace::ASSIGHNSTATEMENT ||
		node->getType() == GrammarSymSpace::READVAR)//赋值语句和读变量，使用var
	{
		return -2;
	}
	if (node->getType() == GrammarSymSpace::CALLSTATEMENT)//调用语句，使用procedure
	{
		return -3;
	}
	if (node->getType() == GrammarSymSpace::STATEMENTTABLE)
	{
		if (node->child[0]->getType() == GrammarSymSpace::ID)//赋值语句，使用var
			return -2;
		if (node->child[1]->getType() == GrammarSymSpace::ID)//调用语句，使用procedure
			return -3;
		return 0;//错误
	}
	if (node->getType() == GrammarSymSpace::FACTOR)
	{
		return -4;//可以是const也可以是var
	}
	if (node == syntaxTree)//找到根节点
		return 0;
	return getUseType(node->getFather());//继续递归向父节点查找
}

void Parser::genPCode()
{
	AstNode* currentNode = syntaxTree;
	sTable* currentTable = runTable;
	pcode.push_back(pCode("JMP", 0, 0x3f3f3f));
	while (1)
	{
		if (currentNode->getType() == GrammarSymSpace::PROCEDUREBODY)
		{
			currentTable = new sTable(currentTable);
		}




		if (currentNode->getType() == GrammarSymSpace::ID)//如果当前节点是ID，向上递归查找是声明还是使用
		{
			int flag = getUseType(currentNode);
			if (flag == 1)
			{
				currentTable->addConst(currentNode->getInfo(), stoi(currentNode->getFather()->child[2]->getInfo()));
				//cout << "const声明成功\n";
			}
			else if (flag == 2)
			{
				currentTable->addVar(currentNode->getInfo());
				//cout << "var声明成功\n";
			}
			else if (flag == 3)
			{
				currentTable/*->getPre()*/->addProcedure(currentNode->getInfo());
				//cout << "procedure声明成功\n";
			}
			else
			{
				errorAdd(currentNode->getPos(), "Unknown error.");
			}
		}

		else if (currentNode->getType() == GrammarSymSpace::STATEMENT)
		{
			handleStatement(currentNode, currentTable);
			while (!currentNode->child.empty())
			{
				currentNode = currentNode->child[currentNode->child.size() - 1];
			}
		}

		else if (currentNode->getType() == GrammarSymSpace::STATEMENTTABLE)
		{
			handleStatementTable(currentNode, currentTable);
			while (!currentNode->child.empty())
			{
				currentNode = currentNode->child[currentNode->child.size() - 1];
			}
		}




		//移向下一个节点
		if (!currentNode->child.empty())
		{
			currentNode = currentNode->child[0];
		}
		else
		{
			while (currentNode->getWz() == currentNode->getFather()->child.size() - 1)
			{
				currentNode = currentNode->getFather();
				if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
				{
					//currentTable = currentTable->getPre();
					if (currentNode->getFather()->getType() == GrammarSymSpace::PROCEDUREBODY)
					{
						//currentTable = currentTable->getPre();
						currentTable->getPre()->setProcedure(currentNode->getFather()->getFather()->child[0]->child[1]->getInfo(), pcode.size());//table和procedure id可能定位错误
						pcode.push_back(pCode("INT", 0, 3 + currentTable->getVarSize()));
					}
					else if (currentNode->getFather()->getType() == GrammarSymSpace::DEFINITIONPART)
					{
						startJump = pcode.size();
						pcode[0].a2 = pcode.size();
						pcode.push_back(pCode("INT", 0, 3 + currentTable->getVarSize()));
					}
				}
				if (currentNode->getType() == GrammarSymSpace::PROCEDUREBODY)
				{
					currentTable = currentTable->getPre();
				}
				if (currentNode->getType() == GrammarSymSpace::PROCEDUREBODY ||
					currentNode->getType() == GrammarSymSpace::SUBPROCEDURE)
				{
					pcode.push_back(pCode("OPR", 0, 0));
				}
				if (currentNode == syntaxTree)
				{
					return;
				}
			}
			currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];
		}
	}
}

void Parser::handleExpression(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::TERM)
	{
		handleTerm(currentNode->child[0], currentTable);
		handleExpressionPlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::PLUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		handleExpressionPlus(currentNode->child[2], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::MINUS)
	{
		//ToDo:表达式开头为-时的做法
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 1));
		handleExpressionPlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleExpressionPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::PLUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 2));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::MINUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 3));
	}
	else
	{
		return;
	}
	handleExpressionPlus(currentNode->child[2], currentTable);
}

void Parser::handleTerm(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleFactor(currentNode->child[0], currentTable);
	handleTermPlus(currentNode->child[1], currentTable);
}

void Parser::handleTermPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::TIMES)
	{
		handleFactor(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 4));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::DIVISION)
	{
		handleFactor(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 5));
	}
	else
	{
		return;
	}
	handleTermPlus(currentNode->child[2], currentTable);
}

void Parser::handleFactor(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::LEFTPARENT)
	{
		handleExpression(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::INTEGER)
	{
		pcode.push_back(pCode("LIT", 0, stoi(currentNode->child[0]->getInfo())));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::ID)
	{
		if (currentTable->judgeVarOrConst(currentNode->child[0]->getInfo()))
		{
			pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
			pcode.push_back(pCode("LOD", tmp.pre, tmp.off));
		}
		else
		{
			pcode.push_back(pCode("LIT", 0, currentTable->findConst(currentNode->child[0]->getInfo())));
		}
	}
}

void Parser::handleCondition(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ODD)
	{
		handleExpression(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 6));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::EXPRESSION)
	{
		handleExpression(currentNode->child[0], currentTable);
		handleConditionPlus(currentNode->child[1], currentTable);
	}
}

void Parser::handleConditionPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleExpression(currentNode->child[1], currentTable);
	switch (currentNode->child[0]->getType())
	{
	case GrammarSymSpace::EQUAL:
		pcode.push_back(pCode("OPR", 0, 8));
		return;
	case GrammarSymSpace::NOTEQUAL:
		pcode.push_back(pCode("OPR", 0, 9));
		return;
	case GrammarSymSpace::LESSTHAN:
		pcode.push_back(pCode("OPR", 0, 10));
		return;
	case GrammarSymSpace::GREATEREQUAL:
		pcode.push_back(pCode("OPR", 0, 11));
		return;
	case GrammarSymSpace::GREATERTHAN:
		pcode.push_back(pCode("OPR", 0, 12));
		return;
	case GrammarSymSpace::LESSEQUAL:
		pcode.push_back(pCode("OPR", 0, 13));
		return;
	default:
		break;
	}
}

void Parser::handleStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ASSIGHNSTATEMENT)
	{
		handleAssignStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CALLSTATEMENT)
	{
		handleCallStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::COMPOUNDSTATEMENT)
	{
		handleCompoundStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CONTIDITIONSTATEMENT)
	{
		handleConditionStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::LOOPSTATEMENT)
	{
		handleLoopStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::READSTATEMENT)
	{
		handleReadStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::WRITESTATEMENT)
	{
		handleWriteStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::REPEATSTATEMENT)
	{
		handleRepeatStatement(currentNode->child[0], currentTable);
	}
}

void Parser::handleAssignStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	currentNode = currentNode->child[2];
	handleExpression(currentNode, currentTable);
	pos tmp = currentTable->findVar(currentNode->getFather()->child[0]->getInfo());
	pcode.push_back(pCode("STO", tmp.pre, tmp.off));
}

void Parser::handleCallStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	pcode.push_back(pCode("CAL", currentTable->findProcedure(currentNode->child[1]->getInfo()).pre, currentTable->findProcedure(currentNode->child[1]->getInfo()).off));
}

void Parser::handleCompoundStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleStatementTable(currentNode->child[1], currentTable);
}

void Parser::handleConditionStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleCondition(currentNode->child[1], currentTable);
	pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
	handleStatement(currentNode->child[3], currentTable);
	for (int i = pcode.size() - 1; i > -1; --i)
	{
		if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
		{
			pcode[i].a2 = pcode.size();
			break;
		}
	}
	handleConditionStatementPlus(currentNode->child[4], currentTable);
}

void Parser::handleConditionStatementPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ELSE)
	{
		for (int i = pcode.size() - 1; i > -1; --i)
		{
			if (pcode[i].op == "JPC")
			{
				++pcode[i].a2;
				break;
			}
		}
		pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
		handleStatement(currentNode->child[1], currentTable);
		for (int i = pcode.size() - 1; i > -1; --i)
		{
			if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
			{
				pcode[i].a2 = pcode.size();
				break;
			}
		}
	}
}

void Parser::handleLoopStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	int jump = pcode.size();
	handleCondition(currentNode->child[1], currentTable);
	pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
	//int jump = pcode.size();
	handleStatement(currentNode->child[3], currentTable);
	//handleCondition(currentNode->child[1], currentTable);
	for (int i = pcode.size() - 1; i > -1; --i)
	{
		if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
		{
			pcode[i].a2 = pcode.size() + 1;
			break;
		}
	}
	pcode.push_back(pCode("JMP", 0, jump));
}

void Parser::handleReadStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleReadVarTable(currentNode->child[2], currentTable);
}

void Parser::handleReadVarTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleReadVar(currentNode->child[0], currentTable);
	handleReadVarTablePlus(currentNode->child[1], currentTable);
}

void Parser::handleReadVarTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::COMMA)
	{
		handleReadVar(currentNode->child[1], currentTable);
		handleReadVarTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleReadVar(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
	pcode.push_back(pCode("RED", tmp.pre, tmp.off));
}

void Parser::handleWriteStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleWriteExpressionTable(currentNode->child[2], currentTable);
}

void Parser::handleWriteExpressionTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleExpression(currentNode->child[0], currentTable);
	pcode.push_back(pCode("WRT", 0, 0));
	handleWriteExpressionTablePlus(currentNode->child[1], currentTable);
}

void Parser::handleWriteExpressionTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::COMMA)
	{
		handleExpression(currentNode->child[1], currentTable);
		pcode.push_back(pCode("WRT", 0, 0));
		handleWriteExpressionTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleRepeatStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	int jump = pcode.size();
	handleStatementTable(currentNode->child[1], currentTable);
	handleCondition(currentNode->child[3], currentTable);
	pcode.push_back(pCode("JPC", 0, pcode.size() + 2));
	pcode.push_back(pCode("JMP", 0, jump));
}

void Parser::handleStatementTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ID)
	{
		handleExpression(currentNode->child[2], currentTable);
		pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
		pcode.push_back(pCode("STO", tmp.pre, tmp.off));
		handleStatementTablePlus(currentNode->child[3], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CALL)
	{
		pcode.push_back(pCode("CAL", currentTable->findProcedure(currentNode->child[1]->getInfo()).pre, currentTable->findProcedure(currentNode->child[1]->getInfo()).off));
		handleStatementTablePlus(currentNode->child[2], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::BEGIN)
	{
		handleStatementTable(currentNode->child[1], currentTable);
		handleStatementTablePlus(currentNode->child[3], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CONTIDITIONSTATEMENT)
	{
		handleConditionStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::LOOPSTATEMENT)
	{
		handleLoopStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::READSTATEMENT)
	{
		handleReadStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::WRITESTATEMENT)
	{
		handleWriteStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::REPEATSTATEMENT)
	{
		handleRepeatStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::EMPTY)
	{
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
}

void Parser::handleStatementTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::SEMICOLON)
	{
		handleStatement(currentNode->child[1], currentTable);
		handleStatementTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::printPCode()
{
	for (int i = 0; i < pcode.size(); ++i)
	{
		cout << i << "\t" << pcode[i].op << "  " << pcode[i].a1 << "\t" << pcode[i].a2 << "\n";
	}
}


