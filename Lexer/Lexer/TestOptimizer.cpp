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
}
