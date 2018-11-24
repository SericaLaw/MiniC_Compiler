#pragma once
class DFA
{
private:
	vector<string> symbol_table;
	map<pair<int, int>, int> trans;
	vector<string> actions;
	int start_state;
public:
	DFA();
	~DFA();
	DFA(map<pair<int, int>, int> trans, vector<string> actions);

	void scan(const string & code);
private:
	int move(int state, int step);
};