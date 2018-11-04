#pragma once
#include "RegexOperator.h"
class Regex
{
private:
	string regex = "";
	string postfix = "";
public:
	/* 仅在构造时指定正则表达式，不可在稍后被修改 */
	Regex(string regex);
	string to_postfix();
private:
	/*  对包含正则定义的正则表达式进行预处理，该函数在转为后缀的时候调用 */
	void preprocess();
};

