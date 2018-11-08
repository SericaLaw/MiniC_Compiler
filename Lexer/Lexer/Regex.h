#pragma once
#include "RegexOperator.h"
class Regex
{
private:
	/* ƥ��ģʽ���������������ʽû���﷨���������š��������ƥ�䣬�Ƿ������ַ����Ƿ��﷨����[-z]���� */
	/* ��ʱû�п���ת���ַ������Լ��������������ʽ��������滻����ַ���û��'-'�����������ѭ��...*/
	string pattern = "";
	/* ��׺���ʽ��ʽ */
	string postfix = "";
	/* ���أ���.l�ļ���ȡ�ʷ�����ʱ������ÿ��ģʽ��action��ȡ��Ӧ���أ���ζ��������ʽ��״̬�����ս�ڵ��ȡ���ж� */
	string lexeme = "";
public:
	/* ���ڹ���ʱָ��������ʽ���������Ժ��޸� */
	Regex(string pattern);
	Regex(string pattern, string lexeme);
	string get_lexeme();
	string to_postfix();
	string get_pattern();
private:
	/*  �԰����������������ʽ����Ԥ�����ú�����תΪ��׺��ʱ����� */
	void preprocess();
};

