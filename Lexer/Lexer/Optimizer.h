#pragma once
class Optimizer
{
private:
	vector<set<FANode*>> d_states;
	/* ״̬����ָ�������� ӳ�䵽ת��״̬������ */
	map<pair<int, int>, int> d_trans;
	set<int> alphabet;
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
	/* ������������������Ҫ�Ż� */
	bool set_not_equal(set<FANode*> & left, set<FANode*> & right);
	int not_in_d_states(set<FANode*> & state);
	/* ���õ�dfa������d_states��d_trans�� */
	void build_dfa(NFA & nfa);

	void print_d_trans();
};

