#pragma once
#include <map>
#include <stack>
#include <cassert>
#include <algorithm>
using namespace std;

#include "NFA.h"
class Optimizer
{
private:
	vector<set<FANode*>> d_states;
	/* 接收状态 */
	vector<string> d_actions;
	/* 状态集合索引与输入 映射到转移状态的索引 */
	map<pair<int, int>, int> d_trans;
	set<int> alphabet;

	vector<int> groups;
	map<pair<int, int>, int> min_dfa_trans;
	vector<string> min_dfa_actions;
	int group_count;
public:
	Optimizer();
	~Optimizer();
public:
	//vector<set<FANode*>> get_d_states();
	//map<pair<int, int>, int> get_d_trans();
public:
	/* 龙书97页 */
	static set<FANode*> move(set<FANode*> & T, int a);
	static set<FANode*> epsilon_closure(set<FANode*> & T);
	/* 获取DFA状态在d_states中的索引 */
	int get_index(set<FANode*> & state);
	/* 获取DFA状态的action */
	string get_action(set<FANode*> & state);
	/* 建好的dfa体现在d_states和d_trans中 */
	void build_dfa(NFA & nfa);

	bool recorded_in_min_dfa_trans(int group_num);
	void print_d_trans();

	/* 最小化DFA */
	void minimize_dfa();
	void print_min_dfa_trans();
public:
	map<pair<int, int>, int> get_min_dfa_trans();
	vector<string> get_min_dfa_actions();
};

