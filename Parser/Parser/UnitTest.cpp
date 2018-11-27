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
	parser.add_production("expr : expr '+' expr");
	parser.add_production("expr : expr '-' expr");
	parser.add_production("expr : expr '*' expr");
	parser.add_production("expr : expr '/' expr");
	parser.add_production("expr : 'num'");
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
