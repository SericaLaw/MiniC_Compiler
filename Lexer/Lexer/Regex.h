#pragma once
#include "RegexOperator.h"
class Regex
{
private:
	string regex = "";
	string postfix = "";
public:
	/* ���ڹ���ʱָ��������ʽ���������Ժ��޸� */
	Regex(string regex);
	string to_postfix();
private:
	/*  �԰����������������ʽ����Ԥ�����ú�����תΪ��׺��ʱ����� */
	void preprocess();
};

