#include "stdafx.h"
#include "Optimizer.h"


Optimizer::Optimizer()
{
	min_dfa_trans.clear();
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


bool Optimizer::recorded_in_min_dfa_trans(int group_num)
{
	for (auto it = min_dfa_trans.begin(); it != min_dfa_trans.end(); it++) {
		pair<int, int> item = it->first;
		if (item.first == group_num)
			return true;
	}
	return false;
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
	 * vector<int> groups 记录当前迭代时各个状态对应的组号，迭代前不清空
	 * vector<pair<int, int>> min_dfa_trans 记录各个组的状态转换表，注意到这个转换表最终将变成min dfa的转换表，迭代前清空
	 * vector<string> min_dfa_actions 迭代前清空
	 * vector<int> group_trans_mark 记录各个组在当前迭代中有没有被初始化 - 不需要
	 * has_changed 记录当前迭代有没有新的划分，若为false则迭代结束
	 * 
	 * attention: 终结状态集是否可以正确分组？ 现在还不能！
	 * TODO 下面的算法有问题啊啊啊啊啊啊啊 还是应该保留上一次的min_dfa_trans 龙书p116 新的划分依据和上一次的分组有关！
	*/
	assert(d_states.size() == d_actions.size());
	
	
	map<string,int> marked_actions;
	// init
	group_count = 1;
	for (auto a : d_actions) {
		if (a == "start" || a == "")
			groups.push_back(0);
		else if (a == "phai")
			groups.push_back(group_count++);
		else {
			if (marked_actions.find(a) == marked_actions.end()) {
				groups.push_back(group_count);
				marked_actions[a] = group_count;
				group_count++;
			}
			else {
				groups.push_back(marked_actions[a]);
			}
		}
	}

	assert(groups.size() == d_actions.size());
	
	//assert(groups.size() == d_trans.size());
	bool has_changed = true;


	int loop_count = 1;

	while (has_changed) {
		has_changed = false;
		vector<int> new_group_mark(groups.size(), -1);
		assert(new_group_mark.size() == groups.size());
		for (int i = 0, n = d_states.size(); i < n; i++) {
			if (recorded_in_min_dfa_trans(groups[i])) {
				// check whether state[i] belongs to groups[i]
				bool belong_to_group = true;
				for (int a : alphabet) {
					if (groups[d_trans[pair<int, int>(i, a)]]!= min_dfa_trans[pair<int, int>(groups[i], a)]) {
						// 如果不属于该组，给他一个标记
						new_group_mark[i] = groups[i];
						has_changed = true;
						//belong_to_group = false;
						break;
					}

				}
				// if is, continue

				// else, group[i] = group_count + 1; has_changed = true;
				//if (!belong_to_group) {
					//groups[i] = group_count;
					//has_changed = true;
				//}
			}
			else {
				// init min_dfa_trans for group[i]
				for (int a : alphabet) {
					int trans_state_index = d_trans[pair<int, int>(i, a)];
					min_dfa_trans[pair<int, int>(groups[i], a)] = groups[trans_state_index];
				}
			}					
		}
		// 分配新的组号，对于new_group_mark中不为-1的状态标记，给相同的标记以相同的新的组号，分配好新组号后置位-1
		while (find_if(new_group_mark.begin(), new_group_mark.end(), [](int x) -> bool { return x != -1; }) != new_group_mark.end()) {
			int i = 0;
			while (i < new_group_mark.size() && new_group_mark[i] == -1) {
				i++;
			}
			assert(i < new_group_mark.size());
			int mark_to_change = groups[i];
			for (int j = 0, n = new_group_mark.size(); j < n; j++) {
				if (new_group_mark[j] == mark_to_change) {
					groups[j] = group_count;
					new_group_mark[j] = -1;
				}
			}
			group_count++;
		}
		if (has_changed) {
			// go to next iteration
			//group_count++;
			min_dfa_trans.clear();
		}
		// else algo stops

	}
	for (int i = 0; i < group_count; i++) {
		for (int j = 0, n = groups.size(); j < n; j++) {
			int g = groups[j];
			if (g == i) {
				min_dfa_actions.push_back(d_actions[j]);
				break;
			}
		}
	}
	assert(min_dfa_actions.size() == group_count);

}

void Optimizer::print_min_dfa_trans()
{
	for (int i = 0; i < group_count; i++) {
		cout << "group: " << i << '\t' << "action: " << min_dfa_actions[i] << '\t';
		for (int a : alphabet) {
			cout << (char)a << ": " << min_dfa_trans[pair<int, int>(i, a)] << '\t';
		}
		cout << endl;
	}
}
