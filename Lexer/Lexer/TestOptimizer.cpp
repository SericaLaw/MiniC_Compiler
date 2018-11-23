#include "stdafx.h"
#include "TestOptimizer.h"


TestOptimizer::TestOptimizer()
{
}


TestOptimizer::~TestOptimizer()
{
}

void TestOptimizer::test_move()
{
	NFA nfa;
	nfa.create_from_regex(Regex("a|b|c").to_postfix(), "a|b|c");
	nfa.print();

	set<FANode*> s0;
	s0.insert(nfa.get_start_node());

	set<FANode*> epsilon_closure_s0 = Optimizer::epsilon_closure(s0);
	cout << "epsilon closure of s0 ( epsilon closure of {" << nfa.get_start_node()->get_state_id() << "}):\n";
	for (set<FANode*>::iterator it = epsilon_closure_s0.begin(); it != epsilon_closure_s0.end(); it++)
		cout << (*it)->get_state_id() << ' ';

	cout << "\nIf we read 'a', we can go to :\n";
	set<FANode*> move_a = Optimizer::move(epsilon_closure_s0, 'a');
	set<FANode*> u = Optimizer::epsilon_closure(move_a);

	for (set<FANode*>::iterator it = u.begin(); it != u.end(); it++)
		cout << (*it)->get_state_id() << ' ';

	/* 空集 */
	cout << "\nIf we read 'd', we can go to :\n";
	set<FANode*> move_d = Optimizer::move(epsilon_closure_s0, 'd');
	auto u_d = Optimizer::epsilon_closure(move_d);

	for (auto it = u_d.begin(); it != u_d.end(); it++)
		cout << (*it)->get_state_id() << ' ';

	/* 下面要保证两个空集是一样的 且算法可以对空集计算闭包 */
	cout << "\nIf we read 'e', we can go to :\n";
	set<FANode*> move_e = Optimizer::move(epsilon_closure_s0, 'e');
	auto u_e = Optimizer::epsilon_closure(move_d);

	for (auto it = u_e.begin(); it != u_e.end(); it++)
		cout << (*it)->get_state_id() << ' ';

	if (u_d == u_e)
		cout << "u_d == u_e";

	cout << "\nIf null set read 'e', we can go to :\n";
	set<FANode*> move_null = Optimizer::move(u_d, 'e');
	auto u_null = Optimizer::epsilon_closure(move_null);
	for (auto it = u_null.begin(); it != u_null.end(); it++)
		cout << (*it)->get_state_id() << ' ';

	assert(u_null == u_d);

	//Optimizer o;
	//o.build_dfa(nfa);

}

void TestOptimizer::test_build_dfa()
{
	NFA nfa;
	/*nfa.create_from_regex(Regex(".+(ab|cd|ef)*|(0|1)?ab*").to_postfix(), ".+(ab|cd|ef)*|(0|1)?ab*");*/
	/*nfa.create_from_regex(Regex("ab(aa|bb)c").to_postfix(), "ab(aa|bb)c");*/
	//nfa.create_from_regex(Regex("a|abb|a*b+").to_postfix(), "a|abb|a*b+");
	nfa.create_from_regex(Regex("(a|b)*abb").to_postfix(), "(a|b)*abb");
	//vector<string> regexes;
	//vector<string> actions;
	//regexes.push_back("a");
	//regexes.push_back("abb");
	//regexes.push_back("a*b+");
	//actions.push_back("a");
	//actions.push_back("abb");
	//actions.push_back("a*b+");
	//nfa.create_from_regexes(regexes, actions);
	nfa.print();

	Optimizer o;
	o.build_dfa(nfa);
	o.print_d_trans();
}

void TestOptimizer::test_minimize_dfa()
{
	NFA nfa;
	nfa.create_from_regex(Regex("a|abb|a*b+").to_postfix(), "a|abb|a*b+");
	/*vector<string> regexes;
	vector<string> actions;
	regexes.push_back("a");
	regexes.push_back("abb");
	regexes.push_back("a*b+");
	actions.push_back("a");
	actions.push_back("abb");
	actions.push_back("a*b+");
	nfa.create_from_regexes(regexes, actions);*/
	//nfa.create_from_regex(Regex("(a|b)*a(a|b)").to_postfix(), "(a|b)*abb");
	Optimizer o;
	o.build_dfa(nfa);
	o.print_d_trans();
	cout << endl;
	o.minimize_dfa();
	o.print_min_dfa_trans();
}

void TestOptimizer::test_mini_mini_c()
{
	vector<string> regexes;
	vector<string> actions;
	regexes.push_back("\\+");
	actions.push_back("+");
	regexes.push_back("-");
	actions.push_back("-");
	regexes.push_back("\\*");
	actions.push_back("*");
	regexes.push_back("/");
	actions.push_back("/");

	regexes.push_back("\\+=");
	actions.push_back("+=");
	regexes.push_back("-=");
	actions.push_back("-=");
	regexes.push_back("\\*=");
	actions.push_back("*=");
	regexes.push_back("/=");
	actions.push_back("/=");

	regexes.push_back("&");
	actions.push_back("&");
	regexes.push_back("\\|");
	actions.push_back("\\|");
	regexes.push_back("=");
	actions.push_back("=");

	regexes.push_back("==");
	actions.push_back("eq");
	regexes.push_back("<");
	actions.push_back("lt");
	regexes.push_back(">");
	actions.push_back("gt");
	regexes.push_back("<=");
	actions.push_back("lte");
	regexes.push_back(">=");
	actions.push_back("gte");
	//regexes.push_back(":");
	//actions.push_back(":");
	//regexes.push_back("\\?");
	//actions.push_back("\\?");

	/*regexes.push_back("main");
	actions.push_back("main");
	regexes.push_back("const");
	actions.push_back("const");
	regexes.push_back("void");
	actions.push_back("void");*/

	//regexes.push_back(",");
	//actions.push_back(",");
	//regexes.push_back(";");
	//actions.push_back(";");
	//regexes.push_back("{");
	//actions.push_back("{");
	//regexes.push_back("}");
	//actions.push_back("}");

	NFA nfa;
	nfa.create_from_regexes(regexes, actions);

	Optimizer o;
	o.build_dfa(nfa);
	o.print_d_trans();
	cout << endl;
	o.minimize_dfa();
	o.print_min_dfa_trans();
}
