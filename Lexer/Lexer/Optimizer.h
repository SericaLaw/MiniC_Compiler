#pragma once
class Optimizer
{
private:
	vector<set<FANode*>> d_states;
	/* 接收状态 */
	vector<string> d_actions;
	/* 状态集合指针与输入 映射到转移状态的索引 */
	map<pair<int, int>, int> d_trans;
	set<int> alphabet;
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


	void print_d_trans();
	/* 最小化DFA */
	void minimize_dfa();
};

