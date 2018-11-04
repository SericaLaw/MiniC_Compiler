#include "stdafx.h"
#include "Regex.h"


Regex::Regex(string regex)
{
	this->regex = regex;
}

string Regex::to_postfix()
{
	/* 已经进行过转换则直接返回 */
	if (!postfix.empty())
		return postfix;

	/* 预处理，仅执行一次 */
	preprocess();

	stack<char> op; // 操作符栈
	op.push('$'); // 哨兵
	regex += '$'; // 哨兵

	for (int i = 0, n = regex.size(); i < n && !op.empty();) {
		char current = regex[i];
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

void Regex::preprocess()
{
	for (int i = 1, n = regex.size(); i < n; i++) {
		char former = regex[i - 1];
		char current = regex[i];
		/* 如果当前字符是字母 */
		if (!RegexOperator::includes(current)) {
			if (/* 如果前面的字符是单目运算符或字母，则在当前字符前加上连接运算符 */
				RegexOperator::includes(former) && RegexOperator::typeof(former) == 1 ||
				!RegexOperator::includes(former)) {
				regex.insert(i, "#");
			}
		}
	}
}
