#pragma once
class Tool
{
public:
	Tool();
	~Tool();
public:
	static vector<string> split(const string& str, const string& delim);
};

