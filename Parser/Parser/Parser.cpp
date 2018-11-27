// Parser.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
vector<int> as(1000000, 1);
void trail1() {
	string tmp;
	for (auto a : as) {
		tmp = a;
	}
}

void trail2() {
	string tmp;
	for (auto a = as.begin(), n = as.end(); a != n; ++a) {
		tmp = *a;
	}
}

void trail3() {
	string tmp;
	for (int a = 0, n = as.size(); a < n; ++a) {
		tmp = as[a];
	}
}

int main()
{
	UnitTest::time_it(&UnitTest::test_parse_by_LL_1);
	/*UnitTest::time_it(&trail1);
	UnitTest::time_it(&trail2);
	UnitTest::time_it(&trail3);*/

    return 0;
}

