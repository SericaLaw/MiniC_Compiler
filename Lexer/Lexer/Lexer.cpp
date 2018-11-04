// Lexer.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int main()
{
	Regex regex("ab*c|(d|ef)+");
	Regex test2("(ab|(a+b*|c)*)*");

	cout << regex.to_postfix() << endl
		<< test2.to_postfix() << endl
		<< regex.to_postfix();
	getchar();
    return 0;
}

