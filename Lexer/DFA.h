#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include "../Common/Token.h"
using namespace std;
class DFA
{
private:
	vector<string> symbol_table;
	map<pair<int, int>, int> trans;
	vector<string> actions;
	int start_state;
	vector<Token> tokens;
public:
	DFA();
	~DFA();
	DFA(map<pair<int, int>, int> trans, vector<string> actions);

	bool scan(const string & code);
	vector<Token> get_tokens();
	void output_res(int id, const string& lexeme, const string& word);
	void output_error(int line, int offset, const string&);
private:
	int move(int state, int step);
};