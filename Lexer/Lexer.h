#pragma once
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "NFA.h"
#include "Optimizer.h"
#include "DFA.h"

class Lexer
{
private:
	DFA dfa;
public:
	Lexer();
	~Lexer();
public:
	/* debug */
	void build_mini_c_min_dfa(bool debug);
	void run(bool debug, const char* path_to_code = nullptr);
};

