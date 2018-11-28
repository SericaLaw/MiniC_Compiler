#pragma once
#include <cassert>
#include "SyntaxParser.h"

using namespace std;

class UnitTest
{
public:
	UnitTest();
	~UnitTest();
	
public:
	static SyntaxParser test_add_production();
	static SyntaxParser test_first();
	static SyntaxParser test_follow();
	static SyntaxParser test_build_LL_1_parsing_table();
	static SyntaxParser test_parse_by_LL_1();
	static SyntaxParser test_parse_mini_c();
};