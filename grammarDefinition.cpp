#pragma execution_character_set("utf-8")
#include "pch.h"
#include "grammarDefinition.h"


string GrammarDefinition::GrammarSymTypes[95] =
{
		"程序",
		"分程序",
		"说明部分",
		"常量说明部分",
		"常量定义表",
		"常量定义",
		"变量说明部分",
		"变量表",
		"变量",
		"过程说明列表",
		"过程首部",
		"过程体",
		"语句",
		"赋值语句",
		"过程调用语句",
		"复合语句",
		"条件语句",
		"当型循环语句",
		"读语句",
		"写语句",
		"重复语句",
		"语句表",
		"读变量表",
		"读变量",
		"写表达式表",
		"条件",
		"表达式",
		"项",
		"因子",
		"程序+",
		"分程序+",
		"说明部分+",
		"常量说明部分+",
		"常量定义表+",
		"常量定义+",
		"变量说明部分+",
		"变量表+",
		"变量+",
		"过程说明列表+",
		"过程首部+",
		"过程体+",
		"语句+",
		"赋值语句+",
		"过程调用语句+",
		"复合语句+",
		"条件语句+",
		"当型循环语句+",
		"读语句+",
		"写语句+",
		"重复语句+",
		"语句表+",
		"读变量表+",
		"读变量+",
		"写表达式表+",
		"条件+",
		"表达式+",
		"项+",
		"因子+",
		"表达式++",
		"const",
		"id",
		"integer",
		"var",
		"procedure",
		"call",
		"begin",
		"end",
		"=",
		".",
		";",
		",",
		":=",
		"if",
		"then",
		"else",
		"while",
		"do",
		"read",
		"(",
		")",
		"write",
		"repeat",
		"until",
		"odd",
		"<>",
		"<",
		"<=",
		">",
		">=",
		"+",
		"-",
		"*",
		"/",
		"ε",
		"$"
};


//GrammarDefinition::GrammarDefinition()
//{
//	;
//}
//
//GrammarDefinition::GrammarDefinition(int left)
//{
//	leftpart = left;
//}

GrammarDefinition::GrammarDefinition(int left, vector<int> right)
{
	leftpart = left;
	rightpart = right;
}

bool GrammarDefinition::operator==(const GrammarDefinition & g)
{
	return this->leftpart == g.leftpart && this->rightpart == g.rightpart;
}

bool GrammarDefinition::startWithNonterminal()
{
	if (rightpart.empty() || rightpart[0] > 58)//>58的是终结符
		return false;
	return true;
}

bool GrammarDefinition::theFirstNonterminalOfRightExpressionEqualToAnotherLeftPart(GrammarDefinition g)
{
	if (this->rightpart[0] == g.leftpart)
		return true;
	return false;
}

bool GrammarDefinition::isLeftRecursive()
{
	if (rightpart.empty())
		return false;
	if (leftpart == rightpart[0])
		return true;
	return false;
}

bool GrammarDefinition::isLeftPartEqual(GrammarDefinition g)
{
	if (this->leftpart == g.leftpart)
		return true;
	return false;
}

GrammarDefinition GrammarDefinition::changeTheFirstNonterminalOfRightExpressionToItsProduction()
{
	vector<int> tmp(this->rightpart.begin() + 1, this->rightpart.end());
	tmp.push_back(this->leftPart() + 29);//+29变成相应的PLUS
	return GrammarDefinition(this->leftPart() + 29, tmp);
}

GrammarDefinition GrammarDefinition::changeTheFirstNonterminalOfRightExpressionToItsProduction(GrammarDefinition g)
{

	vector<int>tmp = g.rightpart;

	tmp.insert(tmp.end(), this->rightpart.begin() + 1, this->rightpart.end());
	return GrammarDefinition(this->leftpart, tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminate(GrammarDefinition g)
{
	vector<int> tmp = g.rightpart;
	tmp.push_back(this->leftpart + 29);
	return GrammarDefinition(this->leftpart,tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminatePlus()
{
	vector<int> tmp(this->rightpart.begin() + 1, this->rightpart.end());
	tmp.push_back(this->leftpart + 29);
	return GrammarDefinition(this->leftpart + 29, tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminatePlusEmpty()
{
	return GrammarDefinition(this->leftpart + 29, vector<int>{GrammarSymSpace::EMPTY});
}

ostream & operator<<(ostream & out, const GrammarDefinition & g)
{
	// TODO: 在此处插入 return 语句
	out <<"<"<< g.leftpart << ">" << GrammarDefinition::GrammarSymTypes[g.leftpart] << "\t\t" << "==>";
	for (int i = 0; i < g.rightpart.size(); ++i)
	{
		out <<"<"<<g.rightpart[i]<<">"<< GrammarDefinition::GrammarSymTypes[g.rightpart[i]];
	}
	out << "\n";
	return out;
}
