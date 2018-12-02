#pragma once
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "NFA.h"
#include "Optimizer.h"
#include "DFA.h"
#include "../Common/Token.h"

class Lexer
{
private:
	DFA dfa;
public:
	Lexer();
	~Lexer();
public:
	vector<Token> get_tokens() { return dfa.get_tokens(); }
public:
	/* debug */
	void build_mini_c_min_dfa(bool debug);
	bool run(bool debug, const char* path_to_code = nullptr);
	
};

