#pragma once
class DFA
{
private:
	vector<int> states;
	map<pair<int, int>, int> trans;
	vector<string> actions;
public:
	DFA();
	~DFA();
	DFA(vector<int> & states, map<pair<int, int>, int> & trans, vector<string> & actions);
};