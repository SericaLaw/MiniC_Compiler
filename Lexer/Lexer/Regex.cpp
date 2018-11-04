#include "stdafx.h"
#include "Regex.h"


Regex::Regex(string regex)
{
	this->regex = regex;
}

string Regex::to_postfix()
{
	/* �Ѿ����й�ת����ֱ�ӷ��� */
	if (!postfix.empty())
		return postfix;

	/* Ԥ������ִ��һ�� */
	preprocess();

	stack<char> op; // ������ջ
	op.push('$'); // �ڱ�
	regex += '$'; // �ڱ�

	for (int i = 0, n = regex.size(); i < n && !op.empty();) {
		char current = regex[i];
		/* ���������� */
		if (RegexOperator::includes(current)) {
			/* ��Ŀ�������ֱ����� */
			if (RegexOperator::typeof(current) == 1) {
				postfix += current;
				i++;
				continue;
			}
			/* ˫Ŀ�����������ջ��ջ�����ȼ�ִ���㷨 */
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
		/* ������ֱ����� */
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
		/* �����ǰ�ַ�����ĸ */
		if (!RegexOperator::includes(current)) {
			if (/* ���ǰ����ַ��ǵ�Ŀ���������ĸ�����ڵ�ǰ�ַ�ǰ������������� */
				RegexOperator::includes(former) && RegexOperator::typeof(former) == 1 ||
				!RegexOperator::includes(former)) {
				regex.insert(i, "#");
			}
		}
	}
}
