#pragma once
class RegexOperator
{
public:
	/* �ж��Ƿ���������ʽ����� */
	static bool includes(char op);

	/* �ж��ǵ�Ŀ��˫Ŀ����� */
	static int typeof(char op);

	/* ջ�����ȼ� */
	static int in_comming_priority(char op);

	/* ջ�����ȼ� */
	static int in_stack_priority(char op);
};

