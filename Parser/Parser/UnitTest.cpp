#include "stdafx.h"
#include "UnitTest.h"


UnitTest::UnitTest()
{
}


UnitTest::~UnitTest()
{
}

SyntaxParser UnitTest::test_add_production()
{
	SyntaxParser parser;
	//parser.add_production("expr : expr '+' expr");
	//parser.add_production("expr : expr '-' expr");
	//parser.add_production("expr : expr '*' expr");
	//parser.add_production("expr : expr '/' expr");
	//parser.add_production("expr : 'num'");
	parser.add_production("E : T E`");
	parser.add_production("E` : '+' T E`");
	parser.add_production("E` : 'epsilon'");
	parser.add_production("T : F T`");
	parser.add_production("T` : '*' F T`");
	parser.add_production("T` : 'epsilon'");
	parser.add_production("F : '(' E ')'");
	parser.add_production("F : 'id'");
	parser.print_productions();
	return parser;
}

SyntaxParser UnitTest::test_first()
{
	SyntaxParser parser = test_add_production();
	parser.calc_first();
	parser.print_first_map();
	return parser;
}

SyntaxParser UnitTest::test_follow()
{
	SyntaxParser parser = test_first();
	parser.calc_follow();
	parser.print_follow_map();
	return parser;
}

SyntaxParser UnitTest::test_build_LL_1_parsing_table()
{
	SyntaxParser parser = test_follow();
	parser.build_LL_1_parsing_table();
	parser.print_LL_1_parsing_table();
	return parser;
}

SyntaxParser UnitTest::test_parse_by_LL_1()
{
	SyntaxParser parser = test_build_LL_1_parsing_table();
	vector<string>w;
	w.push_back("id");
	w.push_back("+");
	w.push_back("id");
	w.push_back("*");
	w.push_back("id");
	parser.parse_by_LL_1(w);
	return parser;
}
