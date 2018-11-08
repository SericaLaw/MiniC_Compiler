#include "stdafx.h"
#include "Optimizer.h"


Optimizer::Optimizer()
{
}


Optimizer::~Optimizer()
{
}

set<FANode*> Optimizer::move(set<FANode*> & T, int a)
{
	set<FANode*> a_closure_T;
	
	stack<FANode*> stack;
	for (set<FANode*>::iterator it = T.begin(); it != T.end(); it++) {
		stack.push(*it);
		if (a == FANode::EPSILON)
			a_closure_T.insert(*it);
	}

	while (!stack.empty()) {
		FANode*t = stack.top();
		stack.pop();
		vector<pair<int, FANode*>>* t_trans = t->get_transition_table();

		for (auto u : *t_trans) {
			if (u.first == a && a_closure_T.find(u.second) == a_closure_T.end()) {
				a_closure_T.insert(u.second);
				stack.push(u.second);
			}
		}
	}
	return a_closure_T;
}

set<FANode*> Optimizer::epsilon_closure(set<FANode*> & T)
{
	/*set<FANode*> epsilon_closure_T(T);
	stack<FANode*> stack;
	for (set<FANode*>::iterator it = T.begin(); it != T.end(); it++) {
		stack.push(*it);
	}

	while (!stack.empty()) {
		FANode*t = stack.top();
		stack.pop();
		vector<pair<int, FANode*>>* t_trans = t->get_transition_table();

		for (auto u : *t_trans) {
			if (u.first == FANode::EPSILON && epsilon_closure_T.find(u.second) == epsilon_closure_T.end()) {
				epsilon_closure_T.insert(u.second);
				stack.push(u.second);
			}
		}
	}
	return epsilon_closure_T;*/
	return move(T, FANode::EPSILON);
}

bool Optimizer::set_not_equal(set<FANode*>& left, set<FANode*>& right)
{
	return false;
}

bool Optimizer::not_in_d_states(set<FANode*>& state)
{
	return false;
}

NFA * Optimizer::build_dfa(NFA & nfa, string & alphabet)
{
	FANode * s0 = nfa.get_start_node();
	set<FANode *> t;
	t.insert(s0);
	set<FANode*> t_closure = epsilon_closure(t);
	d_states.push_back(t_closure);

	queue<set<FANode*>*> unmarked;
	unmarked.push(&t_closure);

	while (!unmarked.empty()) {
		set<FANode*>* t = unmarked.front();
		unmarked.pop(); // mark
		for (char a : alphabet) {
			set<FANode*> a_closure = move(*t, (int)a);
			set<FANode*> u = epsilon_closure(a_closure);
			
		}
	}
	return nullptr;
}
