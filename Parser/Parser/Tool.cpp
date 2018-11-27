#include "stdafx.h"
#include "Tool.h"


Tool::Tool()
{
}


Tool::~Tool()
{
}

vector<string> Tool::split(const string & str, const string & delim)
{
	vector<string> res;
	if ("" == str) return  res;

	string strs = str + delim; //*****扩展字符串以方便检索最后一个分隔出的字符串
	size_t pos;
	size_t size = strs.size();

	for (int i = 0; i < size; ++i) {
		pos = strs.find(delim, i); //pos为分隔符第一次出现的位置，从i到pos之前的字符串是分隔出来的字符串
		if (pos < size) { //如果查找到，如果没有查找到分隔符，pos为string::npos
			string s = strs.substr(i, pos - i);//*****从i开始长度为pos-i的子字符串
			if(s.size())
				res.push_back(s);
			i = pos + delim.size() - 1;
		}

	}
	return res;
}
