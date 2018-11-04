#pragma once
class RegexOperator
{
public:
	/* 判断是否是正则表达式运算符 */
	static bool includes(char op);

	/* 判断是单目或双目运算符 */
	static int typeof(char op);

	/* 栈外优先级 */
	static int in_comming_priority(char op);

	/* 栈内优先级 */
	static int in_stack_priority(char op);
};

