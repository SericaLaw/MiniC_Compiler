#include "stdafx.h"
#include "TestRegex.h"


TestRegex::TestRegex()
{
}


TestRegex::~TestRegex()
{
}

void TestRegex::run_testcase()
{
	Regex regex("ab*c|(d|ef)+");
	Regex test2("(ab|(a+b*|c)*)*");
	Regex test3("[._A-Za-z0-9]");
	Regex test4(".+(ab|cd|ef)*|(0|1)?ab*");
	Regex test5("ab(aa|bb)c");
	Regex test6(string("\\+-\\*/\\|\\|\\$"));
	// TODO: ���������E(+|-)��ΪE[+-]����bug ��ʱ��֪��ԭ�� ������Ԥ���������
	// TODO �����(.[0-9]*)��Ϊ(.[0-9]+)����bug ��ΪDFA���ڵ�ִ�л��ǲ������ƥ���ԭ��
	Regex test7("[0-9]+(E(+|-)?[0-9]+)?");
	test6.to_postfix();
	test4.to_postfix();
	test7.to_postfix();
	cout << regex.to_postfix() << endl
		<< test2.to_postfix() << endl
		<< test3.to_postfix() << endl
		<< test4.to_postfix() << endl
		<< test5.to_postfix() << endl
		<< test6.to_postfix() << endl
		<< test7.to_postfix() << endl;

	cout << test3.get_pattern() << endl
		<< test4.get_pattern() << endl
		<< test5.get_pattern() << endl
		<< test6.get_pattern() << endl
		<< test7.get_pattern() << endl;
}
