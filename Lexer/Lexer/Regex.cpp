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
	/* �Ѿ����й�ת����ֱ�ӷ��� */
	if (!postfix.empty())
		return postfix;

	/* patternֻ��һ���ַ�����ֱ�ӷ��� */
	if (pattern.size() == 1) {
		postfix = pattern;
		return postfix;
	}

	/* Ԥ������ִ��һ�� */
	preprocess();

	stack<char> op; // ������ջ
	op.push('$'); // �ڱ�
	pattern += '$'; // �ڱ�

	for (int i = 0, n = pattern.size(); i < n && !op.empty();) {
		char current = pattern[i];
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

string Regex::get_pattern()
{
	return pattern;
}

void Regex::preprocess()
{
	/* ���������ű��ʽ������[._a-zA-z0-9] */
	for (size_t left_bracket_pos = pattern.find_first_of('['); left_bracket_pos != string::npos; left_bracket_pos = pattern.find_first_of('[')) {
		size_t right_bracket_pos = pattern.find_first_of(']');
		/* �ҵ�һ�������ű��ʽ */
		string substr_to_replace = pattern.substr(left_bracket_pos, right_bracket_pos - left_bracket_pos + 1);
		/* һֱ�ҡ�-������'-'�滻Ϊ��ǰ���ַ�֮��������ַ������a-e�е�-�滻Ϊbcd*/
		string replacement = "";
		for (size_t dash_pos = substr_to_replace.find_first_of('-'); 
			dash_pos != string::npos;
			dash_pos = substr_to_replace.find_first_of('-')
		) {
			for (int c = (int)substr_to_replace[dash_pos - 1] + 1; c < (int)substr_to_replace[dash_pos + 1]; c++) {
				replacement += (char)c;
			}
			/* ��replacement�滻��dash_pos��ʼ��1���ַ� */
			/* ��ʱû�п���ת���ַ������Լ��������������ʽ��������滻����ַ���û��'-'�����������ѭ��...*/
			substr_to_replace.replace(dash_pos, 1, replacement);
		}
		/* '-'���滻��ɺ���'|'�ָ��ַ����������β'|'��ΪԲ���ţ�ò��û���ҵ������ĺ���ֻ����ߣ...������ */
		replacement = "(";
		for (int i = 1, n = substr_to_replace.size(); i < n - 1; i++) {
			replacement += substr_to_replace[i];
			replacement += '|';
		}
		replacement[replacement.size() - 1] = ')';
		pattern.replace(left_bracket_pos, right_bracket_pos - left_bracket_pos + 1, replacement);
	}
	/* ��ʽ�������������# */
	for (int i = 1; i < pattern.size(); i++) {
		char former = pattern[i - 1];
		char current = pattern[i];
		/* �����ǰ�ַ�����ĸ�����ţ�ע�ⲻҪ��©���ŵ���� */
		if (!RegexOperator::includes(current) || current == '(' ) {
			if (/* ���ǰ����ַ��ǵ�Ŀ���������ĸ�������ţ����ڵ�ǰ�ַ�ǰ������������� */
				RegexOperator::includes(former) && RegexOperator::typeof(former) == 1 ||
				!RegexOperator::includes(former) ||
				former == ')') {
				pattern.insert(i++, "#");
			}
		}
	}
}
