#pragma once
#include <vector>
using std::vector;

struct Production {
	int head;
	vector<int> body;
	unsigned int index;
	int op;
};