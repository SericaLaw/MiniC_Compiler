#include "stdafx.h"
#include "Regex.h"


Regex::Regex(string pattern)
{
	this->pattern = pattern;
}

Regex::Regex(string pattern, string lexeme)
{
	this->pattern = pattern;
	this->lexeme = lexeme;
}

string Regex::get_lexeme()
{
	return this->lexeme;
}

string Regex::to_postfix()
{
	/* 已经进行过转换则直接返回 */
	if (!postfix.empty())
		return postfix;

	/* pattern只有一个字符，则直接返回 */
	if (pattern.size() == 1) {
		postfix = pattern;
		return postfix;
	}

	/* 预处理，仅执行一次 */
	preprocess();

	stack<char> op; // 操作符栈
	op.push('$'); // 哨兵
	pattern += '$'; // 哨兵

	for (int i = 0, n = pattern.size(); i < n && !op.empty();) {
		char current = pattern[i];
		/* 操作符处理 */
		if (RegexOperator::includes(current)) {
			/* 单目运算符，直接输出 */
			if (RegexOperator::typeof(current) == 1) {
				postfix += current;
				i++;
				continue;
			}
			/* 双目运算符，根据栈内栈外优先级执行算法 */
			char top = op.top();
			if (RegexOperator::in_comming_priority(current) > RegexOperator::in_stack_priority(top)) {
				op.push(current);
				i++;
			} 
			else if (RegexOperator::in_comming_priority(current) < RegexOperator::in_stack_priority(top)) {
				postfix += top;
				op.pop();
			}
			else if (RegexOperator::in_comming_priority(current) == RegexOperator::in_stack_priority(top)) {
				op.pop();
				if (top == '(') 
					i++;
			}
		}
		/* 操作数直接输出 */
		else {
			postfix += current;
			i++;
		}
	}
	
	return postfix;
}

string Regex::get_pattern()
{
	return pattern;
}

void Regex::preprocess()
{
	/* 处理中括号表达式，形如[._a-zA-z0-9] */
	for (size_t left_bracket_pos = pattern.find_first_of('['); left_bracket_pos != string::npos; left_bracket_pos = pattern.find_first_of('[')) {
		size_t right_bracket_pos = pattern.find_first_of(']');
		/* 找到一个中括号表达式 */
		string substr_to_replace = pattern.substr(left_bracket_pos, right_bracket_pos - left_bracket_pos + 1);
		/* 一直找‘-’，把'-'替换为其前后字符之间的所有字符，如把a-e中的-替换为bcd*/
		string replacement = "";
		for (size_t dash_pos = substr_to_replace.find_first_of('-'); 
			dash_pos != string::npos;
			dash_pos = substr_to_replace.find_first_of('-')
		) {
			for (int c = (int)substr_to_replace[dash_pos - 1] + 1; c < (int)substr_to_replace[dash_pos + 1]; c++) {
				replacement += (char)c;
			}
			/* 用replacement替换从dash_pos开始的1个字符 */
			/* 暂时没有考虑转义字符，所以假设输入的正则表达式里，方括号替换后的字符串没有'-'，否则会无限循环...*/
			substr_to_replace.replace(dash_pos, 1, replacement);
		}
		/* '-'都替换完成后，用'|'分割字符串，最后首尾'|'换为圆括号；貌似没有找到这样的函数只能手撸...？？？ */
		replacement = "(";
		for (int i = 1, n = substr_to_replace.size(); i < n - 1; i++) {
			replacement += substr_to_replace[i];
			replacement += '|';
		}
		replacement[replacement.size() - 1] = ')';
		pattern.replace(left_bracket_pos, right_bracket_pos - left_bracket_pos + 1, replacement);
	}
	/* 显式增加连接运算符# */
	for (int i = 1; i < pattern.size(); i++) {
		char former = pattern[i - 1];
		char current = pattern[i];
		/* 如果当前字符是字母或括号，注意不要遗漏括号的情况 */
		if (!RegexOperator::includes(current) || current == '(' ) {
			if (/* 如果前面的字符是单目运算符或字母或右括号，则在当前字符前加上连接运算符 */
				RegexOperator::includes(former) && RegexOperator::typeof(former) == 1 ||
				!RegexOperator::includes(former) ||
				former == ')') {
				pattern.insert(i++, "#");
			}
		}
	}
}
