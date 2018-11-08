#pragma once
class Optimizer
{
private:
	vector<set<FANode*>> d_states;
	map<pair<set<FANode*>*, int>, int> d_trans;
public:
	Optimizer();
	~Optimizer();

public:
	/* ¡˙ È97“≥ */
	static set<FANode*> move(set<FANode*> & T, int a);
	static set<FANode*> epsilon_closure(set<FANode*> & T);
	static bool set_not_equal(set<FANode*> & left, set<FANode*> & right);
	static bool not_in_d_states(set<FANode*> & state);
	NFA* build_dfa(NFA & nfa, string & aplhabet);
};

