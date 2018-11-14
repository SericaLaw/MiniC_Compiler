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
	if (T.size() == 0) {
		// 空集访问任何元素也都是空集
		return  a_closure_T;
	}
	for (set<FANode*>::iterator it = T.begin(); it != T.end(); it++) {
		stack.push(*it);
		if (a == FANode::EPSILON)
			a_closure_T.insert(*it);
	}

	while (!stack.empty()) {
		FANode* t = stack.top();
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
	return left == right;
}

int Optimizer::not_in_d_states(set<FANode*>& state)
{
	for (int i = 0; i < d_states.size(); i++) {
		set<FANode*> s = d_states[i];
		if (s == state)
			return i;
	}
	return -1;
}

void Optimizer::build_dfa(NFA & nfa)
{
	FANode * s0 = nfa.get_start_node();
	set<FANode *> t;
	t.insert(s0);
	set<FANode*> t_closure = epsilon_closure(t);
	d_states.push_back(t_closure);

	vector<bool> marked;
	marked.push_back(false);

	queue<set<FANode*>*> unmarked; // 未标记状态
	unmarked.push(&t_closure);

	alphabet = nfa.get_alphabet();

	/// 如果是空集 则是状态0，这是配合not_in_d_states以及map的性质一起实现的
	for (int i = 0; i < d_states.size(); i++) {
		for (int a : alphabet) {
			set<FANode*> a_closure = move(d_states[i], a);
			set<FANode*> u = epsilon_closure(a_closure);

			int u_index = not_in_d_states(u);
			if (u_index == -1) {
				d_states.push_back(u);
				u_index = not_in_d_states(u);
			}
			d_trans[pair<int, int>(i, a)] = u_index;
	}
	//while (!unmarked.empty()) {
	//	set<FANode*>* t = unmarked.front();  // 未标记状态t
	//	int t_index = not_in_d_states(*t);
	//	assert(t_index >= 0);
	//	unmarked.pop(); // mark
	//	
	//	for (int a : alphabet) {
	//		set<FANode*> a_closure = move(*t, a);
	//		set<FANode*> u = epsilon_closure(a_closure);

	//		int u_index = not_in_d_states(u);
	//		if (u_index == -1) {
	//			d_states.push_back(u);
	//			//u_index = d_states.size() - 1;
	//			u_index = not_in_d_states(u);
	//			assert(u_index == d_states.size());
	//			u = d_states.back();
	//			unmarked.push(&u);
	//		}
	//		d_trans[pair<int, int>(t_index, a)] = d_states.size();
	//	}
	}

	

}

void Optimizer::print_d_trans()
{

	for (auto it = d_trans.begin(); it != d_trans.end(); it++) {
		pair<int, int>first = it->first;
		int second = it->second;
		cout << "Dtran[" << first.first << "," << first.second << "] = " << second << endl;
	}
	for (set<FANode*> state : d_states) {
		int s_index = not_in_d_states(state);
		assert(s_index >= 0);
		cout << "index: " << s_index << "\t";

		cout << "{ ";
		for (FANode* node : state) {
			cout << node->get_state_id() << ' ';
		}
		if (state.size() == 0)
			cout << "Phai";
		cout << "}\t";

		for (int a : alphabet) {
			cout << (char)a << ": " << d_trans[pair<int, int>(s_index, a)] << ' ';
		}
		cout << endl;
	}
}
