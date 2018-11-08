#include "stdafx.h"
#include "TestNFA.h"


TestNFA::TestNFA()
{
}


TestNFA::~TestNFA()
{
}

void TestNFA::run_testcase()
{
	Regex test_join("abc");
	NFA nfa_1;
	nfa_1.create_from_regex(test_join.to_postfix(), test_join.get_pattern());
	nfa_1.print();
	
	cout << "\n------------------------------------\n";

	Regex test_union("a|b|c");
	NFA nfa_2;
	nfa_2.create_from_regex(test_union.to_postfix(), test_union.get_pattern());
	nfa_2.print();

	cout << "\n------------------------------------\n";

	Regex test_star("a*");
	NFA nfa_3;
	nfa_3.create_from_regex(test_star.to_postfix(), test_star.get_pattern());
	nfa_3.print();

	cout << "\n------------------------------------\n";

	Regex test_plus("a+");
	NFA nfa_4;
	nfa_4.create_from_regex(test_plus.to_postfix(), test_plus.get_pattern());
	nfa_4.print();

	cout << "\n------------------------------------\n";

	Regex test_question("a?");
	NFA nfa_5;
	nfa_5.create_from_regex(test_question.to_postfix(), test_question.get_pattern());
	nfa_5.print();

	cout << "\n------------------------------------\n";

	/*Regex test_complex(".+(ab|cd|ef)*|(0|1)?ab*");*/
	Regex test_complex(".+(ab|cd|ef)*|(0|1)?ab*");
	NFA nfa_6;
	nfa_6.create_from_regex(test_complex.to_postfix(), test_complex.get_pattern());
	nfa_6.print();
}
