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
	void output_res(int id, const string &lexeme, const string &word);
	void output_error(int line, int offset, const string &);
private:
	int move(int state, int step);
};