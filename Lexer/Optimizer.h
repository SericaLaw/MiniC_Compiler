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
	/* ����״̬ */
	vector<string> d_actions;
	/* ״̬�������������� ӳ�䵽ת��״̬������ */
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
	/* ����97ҳ */
	static set<FANode*> move(set<FANode*> & T, int a);
	static set<FANode*> epsilon_closure(set<FANode*> & T);
	/* ��ȡDFA״̬��d_states�е����� */
	int get_index(set<FANode*> & state);
	/* ��ȡDFA״̬��action */
	string get_action(set<FANode*> & state);
	/* ���õ�dfa������d_states��d_trans�� */
	void build_dfa(NFA & nfa);

	bool recorded_in_min_dfa_trans(int group_num);
	void print_d_trans();

	/* ��С��DFA */
	void minimize_dfa();
	void print_min_dfa_trans();
public:
	map<pair<int, int>, int> get_min_dfa_trans();
	vector<string> get_min_dfa_actions();
};

