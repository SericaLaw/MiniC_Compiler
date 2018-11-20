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
	return move(T, FANode::EPSILON);
}


int Optimizer::get_index(set<FANode*>& state)
{
	for (int i = 0; i < d_states.size(); i++) {
		set<FANode*> s = d_states[i];
		if (s == state)
			return i;
	}
	return -1;
}

string Optimizer::get_action(set<FANode*>& state)
{
	// state集合里可能有多个action，见龙书p106，这里只遇到第一个就返回，暂时不支持lex的优先级设定
	// TODO: 若要支持，则要在FANode里的action里配置相关优先级（即.l文件里模式出现的次序）
	
	// 空集
	if (state.size() == 0)
		return "phai";


	for (auto s : state) {
		
		if (s->get_action() != "") {
			return s->get_action();
		}

	}
	return "";
}

void Optimizer::build_dfa(NFA & nfa)
{
	FANode * s0 = nfa.get_start_node();
	set<FANode *> t;
	t.insert(s0);
	set<FANode*> t_closure = epsilon_closure(t);
	d_states.push_back(t_closure);

	//string action = get_action(t_closure);
	d_actions.push_back("start");

	alphabet = nfa.get_alphabet();

	for (int i = 0; i < d_states.size(); i++) {
		for (int a : alphabet) {
			set<FANode*> a_closure = move(d_states[i], a);
			set<FANode*> u = epsilon_closure(a_closure);

			int u_index = get_index(u);
			if (u_index == -1) {
				d_states.push_back(u);
				d_actions.push_back(get_action(u));
				u_index = get_index(u);
			}
			d_trans[pair<int, int>(i, a)] = u_index;
		}
	
	}
	
}


void Optimizer::print_d_trans()
{

	//for (auto it = d_trans.begin(); it != d_trans.end(); it++) {
	//	pair<int, int>first = it->first;
	//	int second = it->second;
	//	cout << "Dtran[" << first.first << "," << first.second << "] = " << second << endl;
	//}
	int i = 0;
	for (set<FANode*> state : d_states) {
		int s_index = get_index(state);
		assert(s_index >= 0);
		cout << "index: " << s_index << "\taction:" << d_actions[i++];

		cout << "\t{ ";
		for (FANode* node : state) {
			cout << node->get_state_id() << ' ';
		}
		if (state.size() == 0)
			cout << "Phai";
		cout << " }\t";

		for (int a : alphabet) {
			cout << (char)a << ": " << d_trans[pair<int, int>(s_index, a)] << ' ';
		}
		cout << endl;
	}
}

void Optimizer::minimize_dfa()
{
	/**
	 * map<pair<int, int>, int> d_trans;
	 * vector<set<FANode*>> d_states;
	 * vector<string> d_actions;
	 * 
	 * vector<int> group 记录当前迭代时各个状态对应的组号
	 * vector<pair<int, int>> group_trans 记录各个组的状态转换表，注意到这个转换表最终将变成min dfa的转换表
	 * vector<int> group_trans_mark 记录各个组在当前迭代中有没有被初始化
	 * has_changed 记录当前迭代有没有新的划分，若为false则迭代结束
	 * 
	 * attention: 终结状态集是否可以正确分组？
	*/

	vector<int> groups;
	vector<pair<int, int>> min_dfa_trans;
	vector<int> group_trans_mark;
	bool has_changed = false;

	// init
	for (auto a : d_actions) {
		if (a == "start" || a == "")
			groups.push_back(1);
		else if (a == "phai")
			groups.push_back(2);
		else
			groups.push_back(3);
	}


}
