#pragma once
#include <string>
using std::string;

// �ʷ���Ԫ ���������p69
struct Token {
	Token(string n, string attr, unsigned int i, unsigned int l, unsigned int o)
		: name(n), attribute(attr), index(i), line(l), offset(o) {}
	string name;
	string attribute;
	unsigned int index;
	unsigned int line;
	unsigned int offset;
};