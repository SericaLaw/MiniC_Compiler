#pragma once
#include <cassert>

namespace RegexOperator {
	/* 判断是否是正则表达式运算符 */
	static bool includes(char op)
	{
		if ( /* 括号 */
			op == '(' || op == ')' ||
			/* 单目运算符 */
			op == '*' || op == '+' || op == '?' ||
			/* 双目运算符*/
			op == '#' || op == '|' ||
			/* 特殊占位符 */
			op == '$'
			)
			return true;
		return false;
	}
	/* 判断是单目或双目运算符 */
	static int typeof(char op)
	{
		if (op == '*' || op == '+' || op == '?')
			return 1;
		if (op == '#' || op == '|')
			return 2;
		if (op == '$')
			return 0;
		return 0;
	}

	/**
	| 操作符     | (    | #    | \|   | )    |
	| ---------- | ---- | ---- | ---- | ---- |
	| 栈外优先级 | 6    | 4    | 2    | 1    |
	| 栈内优先级 | 1    | 5    | 3    | 6    |
	*/
	/* 栈外优先级 */
	static int in_comming_priority(char op)
	{
		assert(includes(op));

		if (op == '(')
			return 6;
		if (op == '#')
			return 4;
		if (op == '|')
			return 2;
		if (op == ')')
			return 1;
		if (op == '$')
			return 0;

		return 0;
	}
	/* 栈内优先级 */
	static int in_stack_priority(char op)
	{
		assert(includes(op));

		if (op == '(')
			return 1;
		if (op == '#')
			return 5;
		if (op == '|')
			return 3;
		if (op == ')')
			return 6;
		if (op == '$')
			return 0;

		return 0;
	}

}

